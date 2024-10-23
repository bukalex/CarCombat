// Fill out your copyright notice in the Description page of Project Settings.


#include "MachineGunTurret.h"

AMachineGunTurret::AMachineGunTurret()
{
	JointMesh = CreateDefaultSubobject<UStaticMeshComponent>("Joint Mesh");
	JointMesh->SetupAttachment(BaseMesh);

	GunMesh = CreateDefaultSubobject<UStaticMeshComponent>("Gun Mesh");
	GunMesh->SetupAttachment(JointMesh);
}

void AMachineGunTurret::BeginPlay()
{
	Super::BeginPlay();

}

void AMachineGunTurret::Aim(float DeltaTime)
{
	Super::Aim(DeltaTime);

	if (!Target) return;
	if (!bTargetWithinRotationLimit) return;
	if (!bTargetVisible) return;

	FVector LookDirection = FMath::VInterpNormalRotationTo(
		JointMesh->GetForwardVector(), 
		Target->GetTargetDirection(JointMesh->GetComponentLocation()),
		DeltaTime, 
		RotationSpeed);

	JointMesh->SetWorldRotation(LookDirection.ToOrientationRotator());
}

void AMachineGunTurret::CheckIfLockedOnTarget()
{
	if (!Target)
	{
		bLockedOn = false;
		return;
	}

	bLockedOn = Target->GetAngleToTarget(Target->GetTargetDirection(GunMesh->GetComponentLocation()), GunMesh->GetForwardVector()) < AimingPrecisionAngle;
}

bool AMachineGunTurret::IsTargetWithinRotationLimit()
{
	if (!Target) return false;
	if (Target->GetAngleToTarget(Target->GetTargetDirection(JointMesh->GetComponentLocation()), BaseMesh->GetForwardVector()) > RotationLimit) return false;

	return true;
}

USceneComponent* AMachineGunTurret::GetFiringComponent()
{
	return JointMesh;
}

void AMachineGunTurret::Fire(float DeltaTime)
{
	Super::Fire(DeltaTime);

	if (!bLockedOn) return;
	if (CooldownTimer > 0) return;

	ShotCounter = (ShotCounter + 1) % VFXFrequency;
	CooldownTimer = CooldownDuration;

	FHitResult Hit;
	FCollisionQueryParams CollisionQueryParams = FCollisionQueryParams();
	CollisionQueryParams.AddIgnoredActor(this);

	bool bActorHit = GetWorld()->LineTraceSingleByChannel(
		Hit, 
		GetFiringComponent()->GetComponentLocation(), 
		GetFiringComponent()->GetComponentLocation() + GunMesh->GetForwardVector().
			RotateAngleAxis(-FireAccuracy + rand() % (FireAccuracy * 2 + 1), GunMesh->GetUpVector()).
			RotateAngleAxis(-FireAccuracy + rand() % (FireAccuracy * 2 + 1), GunMesh->GetRightVector()) * Range,
		ECollisionChannel::ECC_Pawn, CollisionQueryParams);
	if (!bActorHit) return;

	IDestroyable* DestroyableActor = Cast<IDestroyable>(Hit.GetActor());
	if (DestroyableActor)
	{
		DestroyableActor->GetDamage(Damage);
	}

	if (ShotCounter == 0)
	{
		ASimpleBullet* VFX = nullptr;
		if (DestroyableActor && HitMetalVFX) VFX = GetWorld()->SpawnActor<ASimpleBullet>(HitMetalVFX, Hit.ImpactPoint, Hit.ImpactNormal.ToOrientationRotator());
		else if (HitGroundVFX) VFX = GetWorld()->SpawnActor<ASimpleBullet>(HitGroundVFX, Hit.ImpactPoint, Hit.ImpactNormal.ToOrientationRotator());
	}
}