// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Interfaces/TurretTargetable.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Cyclone.generated.h"

UCLASS()
class CARCOMBAT_API ACyclone : public APawn, public ITurretTargetable
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ACyclone();

protected:
	int32 WheelCount = 6;
	float CurrentSteering = 0;

	UPROPERTY(EditAnywhere)
	float MovementForce = 500;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* Body;

	UPROPERTY(EditAnywhere)
	TArray<USceneComponent*> WheelComponentGroups;

	UPROPERTY(EditAnywhere)
	TArray<UStaticMeshComponent*> WheelMeshes;

	UPROPERTY(EditAnywhere)
	TArray<UPhysicsConstraintComponent*> WheelConstraints;

	UPROPERTY(EditAnywhere)
	TArray<int32> SteeringWheelNumbers;

	UPROPERTY(EditAnywhere)
	float SteeringRate = 10;

	UPROPERTY(EditAnywhere)
	float SteeringLimit = 25;

	UPROPERTY(EditAnywhere)
	USpringArmComponent* SpringArm;

	UPROPERTY(EditAnywhere)
	UCameraComponent* Camera;

	UPROPERTY(EditAnywhere)
	float CameraRotationRate = 1;

	UPROPERTY(EditAnywhere)
	float CameraMinAngle;

	UPROPERTY(EditAnywhere)
	float CameraMaxAngle;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual FVector GetTargetLocation() override;

	UFUNCTION()
	void MoveForward(float Value);

	UFUNCTION()
	void TurnRight(float Value);

	UFUNCTION()
	void CameraYawInput(float Value);

	UFUNCTION()
	void CameraPitchInput(float Value);
};
