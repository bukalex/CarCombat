// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CarCombat/Interfaces/Destroyable.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Missile.generated.h"

/**
 * 
 */
UCLASS()
class CARCOMBAT_API AMissile : public AActor
{
	GENERATED_BODY()
	
public:
	AMissile();

protected:
	float ElapsedTime = 0;
	float Damage = 0;
	TArray<IDestroyable*> AffectedActors;
	bool bMarkedForDestroy = false;

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

	UPROPERTY(EditAnywhere)
	float Duration = 1;

	virtual void Tick(float DeltaTime) override;

public:
	void InitializeMissile(UPrimitiveComponent* Target, float Damage, AActor* Owner);

	UFUNCTION()
	void OnTriggerEnter(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnExplosionZoneEnter(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnExplosionZoneExit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
