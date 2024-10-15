// Fill out your copyright notice in the Description page of Project Settings.


#include "FireSpring.h"

// Sets default values
AFireSpring::AFireSpring()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>("Body Mesh");
	RootComponent = BodyMesh;

	Emitter = CreateDefaultSubobject<UParticleSystemComponent>("Emitter");
	Emitter->SetupAttachment(BodyMesh);
	Emitter->Deactivate();

	Trigger = CreateDefaultSubobject<UCapsuleComponent>("Trigger");
	Trigger->SetupAttachment(BodyMesh);
	Trigger->OnComponentBeginOverlap.AddDynamic(this, &AFireSpring::OnTriggerEnter);
	Trigger->OnComponentEndOverlap.AddDynamic(this, &AFireSpring::OnTriggerExit);
	Trigger->ComponentTags.Add("Trigger");
}

// Called when the game starts or when spawned
void AFireSpring::BeginPlay()
{
	Super::BeginPlay();
	
	MinInterval = MinInterval >= 0 ? MinInterval : 0;
	MaxInterval = MaxInterval >= MinInterval ? MaxInterval : MinInterval;
	Interval = FMath::RandRange(MinInterval, MaxInterval);

	if (MinInterval == 0)
	{
		Emitter->Activate();
		PrimaryActorTick.bCanEverTick = false;
	}
}

// Called every frame
void AFireSpring::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	EmissionCycle(DeltaTime);
	BurnTick(DeltaTime);
}

void AFireSpring::EmissionCycle(float DeltaTime)
{
	ElapsedTime += DeltaTime;
	if (Emitter->bWasDeactivated)
	{
		if (ElapsedTime >= Interval)
		{
			Emitter->Activate();
			Emitter->ResetNextTick();
			ElapsedTime = 0;
		}
	}
	else if (ElapsedTime >= Duration)
	{
		Emitter->Deactivate();
		ElapsedTime = 0;
		Interval = FMath::RandRange(MinInterval, MaxInterval);
	}
}

void AFireSpring::BurnTick(float DeltaTime)
{
	if (!Target) return;
	if (Emitter->bWasDeactivated) return;

	Target->GetDamage(Damage * DeltaTime);
}

void AFireSpring::OnTriggerEnter(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!Cast<IDestroyable>(OtherActor)) return;

	Target = Cast<IDestroyable>(OtherActor);
}

void AFireSpring::OnTriggerExit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!Target) return;
	if (Target != Cast<IDestroyable>(OtherActor)) return;

	Target = nullptr;
}