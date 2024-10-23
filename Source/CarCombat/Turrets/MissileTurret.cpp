// Fill out your copyright notice in the Description page of Project Settings.


#include "MissileTurret.h"

AMissileTurret::AMissileTurret()
{
	LauncherMesh = CreateDefaultSubobject<UStaticMeshComponent>("Launcher Mesh");
	LauncherMesh->SetupAttachment(BaseMesh);
}

void AMissileTurret::Aim(float DeltaTime)
{
	Super::Aim(DeltaTime);

	if (!Target) return;
	if (!bTargetWithinRotationLimit) return;
	if (!bTargetVisible) return;

	FVector LookDirection = FMath::VInterpNormalRotationTo(
		LauncherMesh->GetForwardVector(),
		Target->GetTargetDirection(LauncherMesh->GetComponentLocation()),
		DeltaTime,
		RotationSpeed);
	FVector BaseLookDirection = FVector::VectorPlaneProject(LookDirection, BaseMesh->GetUpVector());
	FVector LauncherLookDirection = FVector::VectorPlaneProject(LookDirection, BaseMesh->GetRightVector());

	BaseMesh->SetWorldRotation(BaseLookDirection.ToOrientationRotator());
	LauncherMesh->SetWorldRotation(LauncherLookDirection.ToOrientationRotator());
}

void AMissileTurret::CheckIfLockedOnTarget()
{
	if (!Target)
	{
		bLockedOn = false;
		return;
	}

	bLockedOn = Target->GetAngleToTarget(
		FVector::VectorPlaneProject(Target->GetTargetDirection(LauncherMesh->GetComponentLocation()), BaseMesh->GetRightVector()),
		LauncherMesh->GetForwardVector()) < AimingPrecisionAngle;
}

bool AMissileTurret::IsTargetWithinRotationLimit()
{
	if (!Target) return false;

	float Angle = Target->GetAngleToTarget(
		Target->GetTargetDirection(LauncherMesh->GetComponentLocation()),
		FVector::VectorPlaneProject(Target->GetTargetDirection(LauncherMesh->GetComponentLocation()), BaseMesh->GetUpVector()));
	if (RotationLimit < Angle && Angle < 180 - RotationLimit) return false;

	return true;
}

USceneComponent* AMissileTurret::GetFiringComponent()
{
	return LauncherMesh;
}

void AMissileTurret::Fire(float DeltaTime)
{
	Super::Fire(DeltaTime);

	if (!MissileClass) return;
	if (!bLockedOn) return;
	if (CooldownTimer > 0) return;

	CooldownTimer = CooldownDuration;
	AMissile* Missile = GetWorld()->SpawnActor<AMissile>(MissileClass, LauncherMesh->GetComponentLocation(), LauncherMesh->GetComponentRotation());

	if (Missile) Missile->InitializeMissile(Target->GetRootComponent(), Damage, this);
}