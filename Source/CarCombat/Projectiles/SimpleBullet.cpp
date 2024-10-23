// Fill out your copyright notice in the Description page of Project Settings.


#include "SimpleBullet.h"

ASimpleBullet::ASimpleBullet()
{
	PrimaryActorTick.bCanEverTick = true;

	Emitter = CreateDefaultSubobject<UParticleSystemComponent>("Emitter");
	Emitter->Deactivate();
	RootComponent = Emitter;
}

// Called every frame
void ASimpleBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ElapsedTime += DeltaTime;
	if (ElapsedTime >= Duration)
	{
		Destroy();
	}
}

void ASimpleBullet::BeginPlay()
{
	Super::BeginPlay();

	Emitter->Activate();
	Emitter->ResetNextTick();

	AddActorLocalOffset(LocationOffset);
	AddActorLocalRotation(RotationOffset);
}