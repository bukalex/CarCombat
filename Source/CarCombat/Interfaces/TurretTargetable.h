// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "TurretTargetable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UTurretTargetable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class CARCOMBAT_API ITurretTargetable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual FVector GetTargetLocation() = 0;

	virtual FVector GetTargetDirection(const FVector& TurretLocation, bool bNormalized = true);
	virtual float GetAngleToTarget(const FVector& TargetDirection, const FVector& LookVector, bool bGetInDegrees = true);
};
