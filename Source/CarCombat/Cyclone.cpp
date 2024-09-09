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
	
	PlayerInputComponent->BindAxis("CameraYaw", this, &ACyclone::CameraYawInput);
	PlayerInputComponent->BindAxis("CameraPitch", this, &ACyclone::CameraPitchInput);
}

void ACyclone::MoveForward(float Value)
{
	if (Value == 0) return;

	for (UStaticMeshComponent* WheelMesh : WheelMeshes)
	{
		WheelMesh->AddAngularImpulseInRadians(WheelMesh->GetRightVector() * FMath::Sign(FVector::DotProduct(WheelMesh->GetRightVector(), Body->GetRightVector())) * Value * MovementForce);
	}
}

void ACyclone::TurnRight(float Value)
{
	if (Value == 0) return;

	float DeltaAngle = SteeringRate * Value * FApp::GetDeltaTime();
	if (FMath::Abs(CurrentSteering + DeltaAngle) > SteeringLimit) return;

	CurrentSteering += DeltaAngle;
	for (int32 number : SteeringWheelNumbers)
	{
		//WheelConstraints[number - 1]->AddLocalRotation(FQuat(Body->GetUpVector(), DeltaAngle));
		//WheelMeshes[number - 1]->AddLocalRotation(FQuat(Body->GetUpVector(), DeltaAngle));
	}
}

void ACyclone::CameraYawInput(float Value)
{
	if (Value == 0) return;

	SpringArm->AddRelativeRotation(FQuat(FVector(0, 0, 1), CameraRotationRate * Value * FApp::GetDeltaTime()));
}

void ACyclone::CameraPitchInput(float Value)
{
	if (Value == 0) return;

	float DeltaAngle = CameraRotationRate * Value * FApp::GetDeltaTime();
	if (Camera->GetRelativeRotation().Pitch < CameraMinAngle && DeltaAngle > 0) return;
	if (Camera->GetRelativeRotation().Pitch > CameraMaxAngle && DeltaAngle < 0) return;

	Camera->AddLocalRotation(FQuat(FVector(0, 1, 0), DeltaAngle));
}