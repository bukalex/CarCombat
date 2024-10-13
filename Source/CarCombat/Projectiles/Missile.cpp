// Fill out your copyright notice in the Description page of Project Settings.


#include "Missile.h"

AMissile::AMissile()
{
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	RootComponent = Mesh;

	Trigger = CreateDefaultSubobject<UCapsuleComponent>("Trigger");
	Trigger->SetupAttachment(Mesh);
	Trigger->OnComponentBeginOverlap.AddDynamic(this, &AMissile::OnTriggerEnter);
	Trigger->Deactivate();

	ExplosionZone = CreateDefaultSubobject<USphereComponent>("ExplosionZone");
	ExplosionZone->SetupAttachment(Mesh);
	ExplosionZone->OnComponentBeginOverlap.AddDynamic(this, &AMissile::OnExplosionZoneEnter);
	ExplosionZone->OnComponentEndOverlap.AddDynamic(this, &AMissile::OnExplosionZoneExit);
	ExplosionZone->Deactivate();

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

void AMissile::TakeFromPool()
{
	Super::TakeFromPool();

	SmokeEmitter->Activate();
	SmokeEmitter->ResetNextTick();
	Trigger->Activate();
	ExplosionZone->Activate();
	Mesh->SetVisibility(true);
}

void AMissile::SetTarget(UPrimitiveComponent* Target)
{
	MovementComponent->HomingTargetComponent = Target;
	MovementComponent->Velocity = Mesh->GetForwardVector() * MovementComponent->InitialSpeed;
}

void AMissile::OnTriggerEnter(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (IsInPool()) return;
	if (OtherActor == ProjectileOwner) return;
	if (!OtherComp->IsVisible()) return;

	for (IDestroyable* Actor : AffectedActors)
	{
		//Actor->GetDamage();
	}

	ReturnToPool();
}

void AMissile::OnExplosionZoneEnter(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (IsInPool()) return;
	if (OtherActor == ProjectileOwner) return;
	if (!OtherComp->IsVisible()) return;
	if (!Cast<IDestroyable>(OtherActor)) return;

	AffectedActors.Add(Cast<IDestroyable>(OtherActor));
}

void AMissile::OnExplosionZoneExit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AffectedActors.Remove(Cast<IDestroyable>(OtherActor));
}

void AMissile::ReturnToPool()
{
	bInPool = true;
	Mesh->SetVisibility(false);

	SmokeEmitter->Deactivate();
	ExplosionEmitter->Activate();
	ExplosionEmitter->ResetNextTick();

	Trigger->Deactivate();
	ExplosionZone->Deactivate();
}