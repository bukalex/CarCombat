// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TurretBase.h"
#include "MachineGunTurret.generated.h"

/**
 * 
 */
UCLASS()
class CARCOMBAT_API AMachineGunTurret : public ATurretBase
{
	GENERATED_BODY()
	
public:
	AMachineGunTurret();

protected:
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* JointMesh;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* GunMesh;
};
