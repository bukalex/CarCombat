// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CircularSaw.generated.h"

UCLASS()
class CARCOMBAT_API ACircularSaw : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACircularSaw();

protected:
	UPROPERTY(EditAnywhere)
	USkeletalMeshComponent* SawMesh;

	UPROPERTY(EditAnywhere)
	UAnimationAsset* RotationAnim;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* Collider;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnColliderOverlap(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};
