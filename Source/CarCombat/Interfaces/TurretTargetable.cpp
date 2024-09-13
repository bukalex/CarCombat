// Fill out your copyright notice in the Description page of Project Settings.


#include "TurretTargetable.h"

// Add default functionality here for any ITurretTargetable functions that are not pure virtual.
FVector ITurretTargetable::GetTargetDirection(const FVector& TurretLocation, bool bNormalized)
{
	FVector TargetDirection = GetTargetLocation() - TurretLocation;

	if (bNormalized) return TargetDirection.GetSafeNormal();
	else return TargetDirection;
}

float ITurretTargetable::GetAngleToTarget(const FVector& TargetDirection, const FVector& LookVector, bool bGetInDegrees)
{
	float DotProduct = LookVector.Dot(TargetDirection);
	float Angle = FMath::Acos(DotProduct);

	if (bGetInDegrees) return FMath::RadiansToDegrees(Angle);
	else return Angle;
}