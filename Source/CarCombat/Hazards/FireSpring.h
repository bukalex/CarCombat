// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CarCombat/Interfaces/TurretTargetable.h"
#include "Components/CapsuleComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FireSpring.generated.h"

UCLASS()
class CARCOMBAT_API AFireSpring : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFireSpring();

protected:
	float ElapsedTime = 0;
	float Interval = 0;
	ITurretTargetable* Target;

	UPROPERTY(EditAnywhere)
	float Duration;

	UPROPERTY(EditAnywhere)
	float MinInterval;

	UPROPERTY(EditAnywhere)
	float MaxInterval;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* BodyMesh;

	UPROPERTY(EditAnywhere)
	UParticleSystemComponent* Emitter;

	UPROPERTY(EditAnywhere)
	UCapsuleComponent* Trigger;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void EmissionCycle(float DeltaTime);
	void BurnTick(float DeltaTime);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnTriggerEnter(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnTriggerExit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
