// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectilePool.h"

// Sets default values
AProjectilePool::AProjectilePool()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AProjectilePool::BeginPlay()
{
	Super::BeginPlay();
	
	if (ProjectileClass)
	{
		TArray<AActor*> Temp;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ProjectileClass, Temp);

		for (AActor* Actor : Temp)
		{
			Projectiles.Add(Cast<AProjectile>(Actor));
		}
	}	
}

// Called every frame
void AProjectilePool::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

AProjectile* AProjectilePool::GetProjectile(AActor* ProjectileOwner)
{
	if (Projectiles.IsEmpty()) return nullptr;

	for (AProjectile* Projectile : Projectiles)
	{
		if (Projectile->IsInPool())
		{
			Projectile->SetProjectileOwner(ProjectileOwner);
			Projectile->TakeFromPool();
			return Projectile;
		}
	}

	return nullptr;
}