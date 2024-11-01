// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Destroyable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UDestroyable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class CARCOMBAT_API IDestroyable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
protected:
	float Health = 100;

public:
	virtual void GetDamage(float Damage);
};
