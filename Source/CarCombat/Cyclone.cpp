// Fill out your copyright notice in the Description page of Project Settings.


#include "Cyclone.h"

// Sets default values
ACyclone::ACyclone()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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
		WheelMeshes.Last()->OnComponentHit.AddDynamic(this, &ACyclone::OnWheelGrounded);

		WheelGroundCheckers.Add(false);
		WheelLastContactTimers.Add(0);

		WheelConstraints.Add(CreateDefaultSubobject<UPhysicsConstraintComponent>(FName("Physics Constraint " + FString::FromInt(i + 1))));
		WheelConstraints.Last()->SetupAttachment(WheelComponentGroups.Last());

		WheelConstraints.Last()->ComponentName1.ComponentName = WheelMeshes.Last()->GetFName();
		WheelConstraints.Last()->ComponentName2.ComponentName = Body->GetFName();

		if (!SteeringWheelNumbers.Contains(i + 1)) continue;

		SteeringConstraints.Add(CreateDefaultSubobject<UPhysicsConstraintComponent>(FName("Steering Constraint " + FString::FromInt(i + 1))));
		SteeringConstraints.Last()->SetupAttachment(WheelComponentGroups.Last());

		SteeringConstraints.Last()->ComponentName1.ComponentName = WheelMeshes.Last()->GetFName();
		SteeringConstraints.Last()->ComponentName2.ComponentName = Body->GetFName();
	}

	MachineGunJointMesh = CreateDefaultSubobject<UStaticMeshComponent>("Machine Gun Joint");
	MachineGunJointMesh->SetupAttachment(Body);

	MachineGunMesh = CreateDefaultSubobject<UStaticMeshComponent>("Machine Gun");
	MachineGunMesh->SetupAttachment(MachineGunJointMesh);

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

	for (int32 i = 0; i < WheelCount; i++)
	{
		WheelLastContactTimers[i] += DeltaTime;
		if (WheelLastContactTimers[i] > 0.1f) WheelGroundCheckers[i] = false;
	}

	GetAimLocation();
	Aim(DeltaTime);
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
		if (!WheelGroundCheckers[WheelMeshes.IndexOfByKey(WheelMesh)]) continue;

		Body->AddForceAtLocation(
			WheelMesh->GetRightVector().Cross(Body->GetUpVector()) * FMath::Sign(FVector::DotProduct(WheelMesh->GetRightVector(), Body->GetRightVector())) * Value * MovementForce,
			WheelMesh->GetComponentLocation());
	}

	Body->SetPhysicsLinearVelocity(Body->GetComponentVelocity().GetClampedToMaxSize(MaxVelocity));
}

void ACyclone::TurnRight(float Value)
{
	float DeltaAngle = SteeringRate * FApp::GetDeltaTime() * (Value != 0 ? Value : -FMath::Sign(CurrentSteering));
	CurrentSteering += DeltaAngle;
	CurrentSteering = FMath::Clamp(CurrentSteering, -SteeringLimit, SteeringLimit);

	for (UPhysicsConstraintComponent* Constraint : SteeringConstraints)
	{
		Constraint->SetAngularOrientationTarget(FRotator(0, CurrentSteering, 0));
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

void ACyclone::OnWheelGrounded(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	float Angle = FMath::Acos(FVector::UpVector.Dot(Hit.ImpactNormal));
	Angle = FMath::RadiansToDegrees(Angle);

	WheelGroundCheckers[WheelMeshes.IndexOfByKey(HitComponent)] = Angle <= MaxSurfaceAngle;
	WheelLastContactTimers[WheelMeshes.IndexOfByKey(HitComponent)] = 0;
}

FVector ACyclone::GetTargetLocation()
{
	return Body->GetComponentLocation() + Body->GetUpVector() * 20;
}

UPrimitiveComponent* ACyclone::GetRootComponent()
{
	return Body;
}

void ACyclone::GetAimLocation()
{
	if (!GetWorld()) return;

	FHitResult Hit;
	FCollisionQueryParams CollisionQueryParams = FCollisionQueryParams();
	CollisionQueryParams.AddIgnoredActor(this);

	bool bActorHit = GetWorld()->LineTraceSingleByChannel(
		Hit, 
		Camera->GetComponentLocation() + Camera->GetForwardVector() * 100, 
		Camera->GetComponentLocation() + Camera->GetForwardVector() * MaxAimDistance,
		ECollisionChannel::ECC_Pawn, 
		CollisionQueryParams);
	if (!bActorHit) return;

	AimLocation = Hit.ImpactPoint;
}

void ACyclone::Aim(float DeltaTime)
{
	if (AimLocation.IsZero()) return;

	FVector LookDirection = FMath::VInterpNormalRotationTo(
		MachineGunMesh->GetForwardVector(),
		(AimLocation - MachineGunMesh->GetComponentLocation()).GetUnsafeNormal(),
		DeltaTime,
		MachineGunRotationRate);
	FVector BaseLookDirection = FVector::VectorPlaneProject(LookDirection, Body->GetUpVector());
	FVector MachineGunLookDirection = FVector::VectorPlaneProject(LookDirection, MachineGunJointMesh->GetRightVector());

	//MachineGunJointMesh->AddRelativeRotation(FQuat::FindBetweenNormals(MachineGunJointMesh->GetForwardVector(), BaseLookDirection.GetUnsafeNormal()));
	//MachineGunJointMesh->SetRelativeRotation(FQuat(
	//	0, 
	//	0, 
	//	FQuat::FindBetweenNormals(Body->GetForwardVector(), BaseLookDirection.GetUnsafeNormal()).Z,
	//	FQuat::FindBetweenNormals(Body->GetForwardVector(), BaseLookDirection.GetUnsafeNormal()).W));
	//MachineGunMesh->SetRelativeRotation(FQuat(
	//	0, 
	//	FQuat::FindBetweenNormals(MachineGunJointMesh->GetForwardVector(), MachineGunLookDirection).Y,
	//	0, 
	//	FQuat::FindBetweenNormals(MachineGunJointMesh->GetForwardVector(), MachineGunLookDirection).W));
}