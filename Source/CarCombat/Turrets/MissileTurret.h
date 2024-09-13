// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TurretBase.h"
#include "MissileTurret.generated.h"

/**
 * 
 */
UCLASS()
class CARCOMBAT_API AMissileTurret : public ATurretBase
{
	GENERATED_BODY()
	
public:
	AMissileTurret();

protected:
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* LauncherMesh;

	virtual void Aim(float DeltaTime) override;
	virtual void CheckIfLockedOnTarget() override;
	virtual bool IsTargetWithinRotationLimit() override;
	virtual USceneComponent* GetFiringComponent() override;
};
