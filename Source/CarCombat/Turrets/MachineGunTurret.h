// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CarCombat/Projectiles/SimpleBullet.h"
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
	int ShotCounter = -1;

	UPROPERTY(EditAnywhere)
	int VFXFrequency = 5;

	UPROPERTY(EditAnywhere)
	int FireAccuracy = 5;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* JointMesh;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* GunMesh;

	UPROPERTY(EditAnywhere)
	TSubclassOf<ASimpleBullet> HitMetalVFX;

	UPROPERTY(EditAnywhere)
	TSubclassOf<ASimpleBullet> HitGroundVFX;

	virtual void BeginPlay() override;
	virtual void Aim(float DeltaTime) override;
	virtual void CheckIfLockedOnTarget() override;
	virtual bool IsTargetWithinRotationLimit() override;
	virtual USceneComponent* GetFiringComponent() override;

	virtual void Fire(float DeltaTime) override;
};
