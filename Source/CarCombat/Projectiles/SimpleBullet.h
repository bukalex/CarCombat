// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Projectile.h"
#include "SimpleBullet.generated.h"

/**
 * 
 */
UCLASS()
class CARCOMBAT_API ASimpleBullet : public AProjectile
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	ASimpleBullet();

protected:
	float ElapsedTime = 0;

	UPROPERTY(EditAnywhere)
	float Duration = 1;

	UPROPERTY(EditAnywhere)
	UParticleSystemComponent* Emitter;

public:
	virtual void Tick(float DeltaTime) override;
	virtual void TakeFromPool() override;
};
