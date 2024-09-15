// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CarCombat/Interfaces/TurretTargetable.h"
#include "CarCombat/Interfaces/Destroyable.h"
#include "Components/SphereComponent.h"
#include "CableComponent.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HarpoonMine.generated.h"

UCLASS()
class CARCOMBAT_API AHarpoonMine : public AActor, public IDestroyable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHarpoonMine();

protected:
	ITurretTargetable* Target;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* BodyMesh;

	UPROPERTY(EditAnywhere)
	UCableComponent* Cable;

	UPROPERTY(EditAnywhere)
	UPhysicsConstraintComponent* Constraint;

	UPROPERTY(EditAnywhere)
	USphereComponent* Trigger;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnTriggerEnter(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
