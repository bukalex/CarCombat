// Fill out your copyright notice in the Description page of Project Settings.


#include "SimpleBullet.h"

ASimpleBullet::ASimpleBullet()
{
	Emitter = CreateDefaultSubobject<UParticleSystemComponent>("Emitter");
	Emitter->Deactivate();
	RootComponent = Emitter;
}

// Called every frame
void ASimpleBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!IsInPool())
	{
		ElapsedTime += DeltaTime;
		if (ElapsedTime >= Duration)
		{
			ReturnToPool();
			ElapsedTime = 0;
		}
	}
}

void ASimpleBullet::TakeFromPool()
{
	Super::TakeFromPool();

	Emitter->Activate();
	Emitter->ResetNextTick();
}