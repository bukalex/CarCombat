// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ChaosWheeledVehicleMovementComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/SphereComponent.h"
#include "CarCombatMode.h"
#include "Interfaces/Destroyable.h"
#include "Interfaces/TurretTargetable.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "CoreMinimal.h"
#include "WheeledVehiclePawn.h"
#include "Cyclone.generated.h"

UCLASS()
class CARCOMBAT_API ACyclone : public AWheeledVehiclePawn, public ITurretTargetable, public IDestroyable
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ACyclone();

protected:
	float MaxAimDistance = 100000;
	FVector AimLocation = FVector::ZeroVector;
	ACarCombatMode* GameMode;

	UPROPERTY(EditAnywhere)
	USpringArmComponent* SpringArm;

	UPROPERTY(EditAnywhere)
	UCameraComponent* Camera;

	UPROPERTY(EditAnywhere)
	USphereComponent* Radar;

	UPROPERTY(EditAnywhere)
	UInputMappingContext* InputMapping;

	UPROPERTY(EditAnywhere)
	UInputAction* MoveForwardAction;

	UPROPERTY(EditAnywhere)
	UInputAction* TurnRightAction;

	UPROPERTY(EditAnywhere)
	UInputAction* RotateCameraAction;

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
	void MoveForward(const FInputActionValue& Value);

	UFUNCTION()
	void TurnRight(const FInputActionValue& Value);

	UFUNCTION()
	void CameraInput(const FInputActionValue& Value);

	UFUNCTION()
	void OnRadarEnter(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnRadarExit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
