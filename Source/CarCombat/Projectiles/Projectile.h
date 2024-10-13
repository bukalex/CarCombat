// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CarCombat/Interfaces/Destroyable.h"
#include "Particles/ParticleSystemComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

UCLASS()
class CARCOMBAT_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectile();

protected:
	bool bInPool = true;
	AActor* ProjectileOwner;

	UPROPERTY(EditAnywhere)
	FVector LocationOffset;

	UPROPERTY(EditAnywhere)
	FRotator RotationOffset;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void ReturnToPool();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void TakeFromPool();
	virtual void ApplyLocationAndRotationOffset();
	virtual void SetTarget(UPrimitiveComponent* Target);
	virtual void SetProjectileOwner(AActor* NewOwner);
	virtual bool IsInPool();
};
