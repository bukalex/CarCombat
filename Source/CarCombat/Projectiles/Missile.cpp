// Fill out your copyright notice in the Description page of Project Settings.


#include "Missile.h"

AMissile::AMissile()
{
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	RootComponent = Mesh;

	Trigger = CreateDefaultSubobject<UCapsuleComponent>("Trigger");
	Trigger->SetupAttachment(Mesh);
	Trigger->OnComponentBeginOverlap.AddDynamic(this, &AMissile::OnTriggerEnter);
	Trigger->ComponentTags.Add("Trigger");

	ExplosionZone = CreateDefaultSubobject<USphereComponent>("ExplosionZone");
	ExplosionZone->SetupAttachment(Mesh);
	ExplosionZone->OnComponentBeginOverlap.AddDynamic(this, &AMissile::OnExplosionZoneEnter);
	ExplosionZone->OnComponentEndOverlap.AddDynamic(this, &AMissile::OnExplosionZoneExit);

	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("MovementComponent");
	MovementComponent->bIsHomingProjectile = true;
	MovementComponent->bRotationFollowsVelocity = true;
	MovementComponent->SetUpdatedComponent(Mesh);
	MovementComponent->ProjectileGravityScale = 0;

	SmokeEmitter = CreateDefaultSubobject<UParticleSystemComponent>("SmokeEmitter");
	SmokeEmitter->SetupAttachment(Mesh);
	SmokeEmitter->Deactivate();

	ExplosionEmitter = CreateDefaultSubobject<UParticleSystemComponent>("ExplosionEmitter");
	ExplosionEmitter->SetupAttachment(Mesh);
	ExplosionEmitter->Deactivate();
}

void AMissile::InitializeMissile(UPrimitiveComponent* Target, float MissileDamage, AActor* MissileOwner)
{
	MovementComponent->HomingTargetComponent = Target;
	MovementComponent->Velocity = Mesh->GetForwardVector() * MovementComponent->InitialSpeed;

	Damage = MissileDamage;
	Owner = MissileOwner;

	SmokeEmitter->Activate();
	SmokeEmitter->ResetNextTick();
}

void AMissile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bMarkedForDestroy) return;

	ElapsedTime += DeltaTime;
	if (ElapsedTime >= Duration)
	{
		Destroy();
	}
}

void AMissile::OnTriggerEnter(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!Owner) return;
	if (OtherActor == Owner) return;
	if (OtherComp->ComponentHasTag("Trigger")) return;

	ExplosionEmitter->Activate();
	ExplosionEmitter->ResetNextTick();

	for (IDestroyable* Actor : AffectedActors)
	{
		Actor->GetDamage(Damage);
	}

	Mesh->SetVisibility(false);
	SmokeEmitter->Deactivate();
	Trigger->Deactivate();
	ExplosionZone->Deactivate();
	MovementComponent->Deactivate();
	bMarkedForDestroy = true;
}

void AMissile::OnExplosionZoneEnter(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == Owner) return;
	if (OtherComp->ComponentHasTag("Trigger")) return;
	if (!Cast<IDestroyable>(OtherActor)) return;

	AffectedActors.Add(Cast<IDestroyable>(OtherActor));
}

void AMissile::OnExplosionZoneExit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AffectedActors.Remove(Cast<IDestroyable>(OtherActor));
}