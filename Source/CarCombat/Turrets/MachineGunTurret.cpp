// Fill out your copyright notice in the Description page of Project Settings.


#include "MachineGunTurret.h"

AMachineGunTurret::AMachineGunTurret()
{
	JointMesh = CreateDefaultSubobject<UStaticMeshComponent>("Joint Mesh");
	JointMesh->SetupAttachment(BaseMesh);

	GunMesh = CreateDefaultSubobject<UStaticMeshComponent>("Gun Mesh");
	GunMesh->SetupAttachment(JointMesh);
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
	if (!Target) return;

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