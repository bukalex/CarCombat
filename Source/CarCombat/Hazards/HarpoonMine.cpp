// Fill out your copyright notice in the Description page of Project Settings.


#include "HarpoonMine.h"

// Sets default values
AHarpoonMine::AHarpoonMine()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>("Body Mesh");
	RootComponent = BodyMesh;

	Cable = CreateDefaultSubobject<UCableComponent>("Rope");
	Cable->SetupAttachment(BodyMesh);
	Cable->SetVisibility(false);
	
	Constraint = CreateDefaultSubobject<UPhysicsConstraintComponent>("Constraint");
	Constraint->SetupAttachment(BodyMesh);

	Trigger = CreateDefaultSubobject<USphereComponent>("Trigger");
	Trigger->SetupAttachment(BodyMesh);
	Trigger->OnComponentBeginOverlap.AddDynamic(this, &AHarpoonMine::OnTriggerEnter);
	Trigger->ComponentTags.Add("Trigger");
}

// Called when the game starts or when spawned
void AHarpoonMine::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHarpoonMine::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void AHarpoonMine::OnTriggerEnter(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Target) return;
	
	Target = Cast<ITurretTargetable>(OtherActor);
	if (!Target) return;
	
	Cable->SetAttachEndToComponent(OtherComp);
	Cable->SetVisibility(true);
	
	Constraint->SetConstrainedComponents(Target->GetRootComponent(), NAME_None, BodyMesh, NAME_None);
	Constraint->SetConstraintReferencePosition(EConstraintFrame::Frame1, FVector::ZeroVector);
}