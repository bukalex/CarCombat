// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CarCombat/Projectiles/ProjectilePool.h"
#include "CarCombat/Interfaces/TurretTargetable.h"
#include "CarCombat/Interfaces/Destroyable.h"
#include "Components/SphereComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TurretBase.generated.h"

UCLASS()
class CARCOMBAT_API ATurretBase : public AActor, public IDestroyable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATurretBase();

protected:
	ITurretTargetable* Target;
	FColor AimingLineColor = FColor::Red;
	bool bLockedOn = true;
	bool bTargetWithinRotationLimit = false;
	bool bTargetVisible = false;
	float CooldownTimer = 0;

	UPROPERTY(EditAnywhere)
	float Range = 20000;

	UPROPERTY(EditAnywhere)
	float RotationLimit;

	UPROPERTY(EditAnywhere)
	float AimingPrecisionAngle = 1;

	UPROPERTY(EditAnywhere)
	float RotationSpeed = 5;

	UPROPERTY(EditAnywhere)
	float CooldownDuration = 1;

	UPROPERTY(EditAnywhere)
	float Damage = 10;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* BaseMesh;

	UPROPERTY(EditAnywhere)
	USphereComponent* AttackZone;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Fire(float DeltaTime);
	virtual void Aim(float DeltaTime);
	virtual void CheckIfLockedOnTarget();
	virtual bool IsTargetWithinRotationLimit();
	virtual bool IsTargetVisible();
	virtual USceneComponent* GetFiringComponent();
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnAttackZoneEnter(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnAttackZoneExit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
