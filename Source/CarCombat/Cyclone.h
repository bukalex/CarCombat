// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/SphereComponent.h"
#include "CarCombatMode.h"
#include "Interfaces/Destroyable.h"
#include "Interfaces/TurretTargetable.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Cyclone.generated.h"

UCLASS()
class CARCOMBAT_API ACyclone : public APawn, public ITurretTargetable, public IDestroyable
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ACyclone();

protected:
	int32 WheelCount = 6;
	float CurrentSteering = 0;
	float MaxAimDistance = 100000;
	TArray<int32> SteeringWheelNumbers = {1, 4};
	TArray<bool> WheelGroundCheckers;
	TArray<float> WheelLastContactTimers;
	FVector AimLocation = FVector::ZeroVector;
	ACarCombatMode* GameMode;

	UPROPERTY(EditAnywhere)
	float MovementForce = 500;

	UPROPERTY(EditAnywhere)
	float MaxVelocity = 500;

	UPROPERTY(EditAnywhere)
	float MaxSurfaceAngle = 45;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* Body;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* MachineGunJointMesh;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* MachineGunMesh;

	UPROPERTY(EditAnywhere)
	TArray<USceneComponent*> WheelComponentGroups;

	UPROPERTY(EditAnywhere)
	TArray<UStaticMeshComponent*> WheelMeshes;

	UPROPERTY(EditAnywhere)
	TArray<UPhysicsConstraintComponent*> WheelConstraints;

	UPROPERTY(EditAnywhere)
	TArray<UPhysicsConstraintComponent*> SteeringConstraints;

	UPROPERTY(EditAnywhere)
	float SteeringRate = 10;

	UPROPERTY(EditAnywhere)
	float SteeringLimit = 25;

	UPROPERTY(EditAnywhere)
	USpringArmComponent* SpringArm;

	UPROPERTY(EditAnywhere)
	UCameraComponent* Camera;

	UPROPERTY(EditAnywhere)
	USphereComponent* Radar;

	UPROPERTY(EditAnywhere)
	float CameraRotationRate = 1;

	UPROPERTY(EditAnywhere)
	float CameraMinAngle;

	UPROPERTY(EditAnywhere)
	float CameraMaxAngle;

	UPROPERTY(EditAnywhere)
	float MachineGunRotationRate;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void GetAimLocation();
	virtual void Aim(float DeltaTime);
	virtual void ValidateGameMode();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual FVector GetTargetLocation() override;
	virtual UPrimitiveComponent* GetRootComponent() override;
	virtual void GetDamage(float Damage) override;

	UFUNCTION()
	void MoveForward(float Value);

	UFUNCTION()
	void TurnRight(float Value);

	UFUNCTION()
	void CameraYawInput(float Value);

	UFUNCTION()
	void CameraPitchInput(float Value);

	UFUNCTION()
	void OnWheelGrounded(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
	void OnRadarEnter(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnRadarExit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
