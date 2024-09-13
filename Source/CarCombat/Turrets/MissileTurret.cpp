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

	if (!IsTargetWithinRotationLimit())
	{
		AimingLineColor = FColor::Red;
		return;
	}
	if (!IsTargetVisible())
	{
		AimingLineColor = FColor::Red;
		return;
	}
	if (bLockedOn)
	{
		AimingLineColor = FColor::Blue;
		return;
	}
	AimingLineColor = FColor::Green;

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
	if (!Target) return;

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