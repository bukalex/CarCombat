// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "CoreMinimal.h"
#include "Projectile.h"
#include "Missile.generated.h"

/**
 * 
 */
UCLASS()
class CARCOMBAT_API AMissile : public AProjectile
{
	GENERATED_BODY()
	
public:
	AMissile();

protected:
	TArray<IDestroyable*> AffectedActors;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere)
	UCapsuleComponent* Trigger;

	UPROPERTY(EditAnywhere)
	USphereComponent* ExplosionZone;

	UPROPERTY(EditAnywhere)
	UProjectileMovementComponent* MovementComponent;

	UPROPERTY(EditAnywhere)
	UParticleSystemComponent* SmokeEmitter;

	UPROPERTY(EditAnywhere)
	UParticleSystemComponent* ExplosionEmitter;

	virtual void ReturnToPool() override;

public:
	virtual void TakeFromPool() override;
	virtual void SetTarget(UPrimitiveComponent* Target) override;

	UFUNCTION()
	void OnTriggerEnter(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnExplosionZoneEnter(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnExplosionZoneExit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
