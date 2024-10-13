// Fill out your copyright notice in the Description page of Project Settings.


#include "CircularSaw.h"

// Sets default values
ACircularSaw::ACircularSaw()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SawMesh = CreateDefaultSubobject<USkeletalMeshComponent>("Saw Mesh");
	RootComponent = SawMesh;

	Collider = CreateDefaultSubobject<UStaticMeshComponent>("Collider");
	Collider->SetupAttachment(SawMesh);
	Collider->SetVisibility(false);
	Collider->OnComponentHit.AddDynamic(this, &ACircularSaw::OnColliderOverlap);
}

// Called when the game starts or when spawned
void ACircularSaw::BeginPlay()
{
	Super::BeginPlay();
	
	SawMesh->PlayAnimation(RotationAnim, true);
}

// Called every frame
void ACircularSaw::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACircularSaw::OnColliderOverlap(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	IDestroyable* DestroyableActor = Cast<IDestroyable>(OtherActor);
	if (!DestroyableActor) return;

	DestroyableActor->GetDamage(Damage * FApp::GetDeltaTime());
}