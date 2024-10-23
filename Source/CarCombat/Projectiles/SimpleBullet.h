// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Particles/ParticleSystemComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SimpleBullet.generated.h"

/**
 * 
 */
UCLASS()
class CARCOMBAT_API ASimpleBullet : public AActor
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	ASimpleBullet();

protected:
	float ElapsedTime = 0;

	UPROPERTY(EditAnywhere)
	FVector LocationOffset;

	UPROPERTY(EditAnywhere)
	FRotator RotationOffset;

	UPROPERTY(EditAnywhere)
	float Duration = 1;

	UPROPERTY(EditAnywhere)
	UParticleSystemComponent* Emitter;

	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
};
