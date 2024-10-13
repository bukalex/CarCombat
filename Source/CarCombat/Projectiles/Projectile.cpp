// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SetHidden(true);
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AProjectile::ReturnToPool()
{
	bInPool = true;
	SetHidden(true);
}

void AProjectile::TakeFromPool()
{
	bInPool = false;
	SetHidden(false);
}

void AProjectile::ApplyLocationAndRotationOffset()
{
	AddActorLocalOffset(LocationOffset);
	AddActorLocalRotation(RotationOffset);
}

void AProjectile::SetTarget(UPrimitiveComponent* Target)
{
	
}

void AProjectile::SetProjectileOwner(AActor* NewOwner)
{
	ProjectileOwner = NewOwner;
}

bool AProjectile::IsInPool()
{
	return bInPool;
}