// Fill out your copyright notice in the Description page of Project Settings.


#include "Cyclone.h"

// Sets default values
ACyclone::ACyclone()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Body = CreateDefaultSubobject<UStaticMeshComponent>("Body Mesh");
	Body->SetSimulatePhysics(true);
	RootComponent = Body;

	for (int32 i = 0; i < WheelCount; i++)
	{
		WheelComponentGroups.Add(CreateDefaultSubobject<USceneComponent>(FName("Wheel " + FString::FromInt(i + 1))));
		WheelComponentGroups.Last()->SetupAttachment(Body);

		WheelMeshes.Add(CreateDefaultSubobject<UStaticMeshComponent>(FName("Wheel Mesh " + FString::FromInt(i + 1))));
		WheelMeshes.Last()->SetupAttachment(WheelComponentGroups.Last());
		WheelMeshes.Last()->SetSimulatePhysics(true);

		WheelConstraints.Add(CreateDefaultSubobject<UPhysicsConstraintComponent>(FName("Physics Constraint " + FString::FromInt(i + 1))));
		WheelConstraints.Last()->SetupAttachment(WheelComponentGroups.Last());

		WheelConstraints.Last()->ComponentName1.ComponentName = WheelMeshes.Last()->GetFName();
		WheelConstraints.Last()->ComponentName2.ComponentName = Body->GetFName();
	}

	SpringArm = CreateDefaultSubobject<USpringArmComponent>("Spring Arm");
	SpringArm->SetupAttachment(Body);

	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(SpringArm);
}

// Called when the game starts or when spawned
void ACyclone::BeginPlay()
{
	Super::BeginPlay();
	
	MovementForce *= Body->GetMass();
}

// Called every frame
void ACyclone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACyclone::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ACyclone::MoveForward);
	PlayerInputComponent->BindAxis("TurnRight", this, &ACyclone::TurnRight);
}

void ACyclone::MoveForward(float Value)
{

}

void ACyclone::TurnRight(float Value)
{

}