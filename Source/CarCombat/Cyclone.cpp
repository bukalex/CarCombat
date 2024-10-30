// Fill out your copyright notice in the Description page of Project Settings.


#include "Cyclone.h"

// Sets default values
ACyclone::ACyclone()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>("Spring Arm");
	SpringArm->SetupAttachment(GetMesh());

	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(SpringArm);

	Radar = CreateDefaultSubobject<USphereComponent>("Radar");
	Radar->SetupAttachment(GetMesh());
	Radar->OnComponentBeginOverlap.AddDynamic(this, &ACyclone::OnRadarEnter);
	Radar->OnComponentEndOverlap.AddDynamic(this, &ACyclone::OnRadarExit);
	Radar->ComponentTags.Add("Trigger");
}

// Called when the game starts or when spawned
void ACyclone::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACyclone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	GetAimLocation();
	Aim(DeltaTime);
}

// Called to bind functionality to input
void ACyclone::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (EnhancedInput)
	{
		EnhancedInput->BindAction(MoveForwardAction, ETriggerEvent::Triggered, this, &ACyclone::MoveForward);
		EnhancedInput->BindAction(TurnRightAction, ETriggerEvent::Triggered, this, &ACyclone::TurnRight);
		EnhancedInput->BindAction(RotateCameraAction, ETriggerEvent::Triggered, this, &ACyclone::CameraInput);
	}

	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
		if (Subsystem)
		{
			Subsystem->AddMappingContext(InputMapping, 0);
		}
	}
}

void ACyclone::MoveForward(const FInputActionValue& Value)
{
	GetVehicleMovement()->SetThrottleInput(Value.Get<float>());
}

void ACyclone::TurnRight(const FInputActionValue& Value)
{

}

void ACyclone::CameraInput(const FInputActionValue& Value)
{
	FVector2D VectorValue = Value.Get<FVector2D>();

	SpringArm->AddRelativeRotation(FQuat(FVector(0, 0, 1), CameraRotationRate * VectorValue.X * FApp::GetDeltaTime()));

	float DeltaAngle = CameraRotationRate * -VectorValue.Y * FApp::GetDeltaTime();
	if (Camera->GetRelativeRotation().Pitch < CameraMinAngle && DeltaAngle > 0) return;
	if (Camera->GetRelativeRotation().Pitch > CameraMaxAngle && DeltaAngle < 0) return;
	
	Camera->AddLocalRotation(FQuat(FVector(0, 1, 0), DeltaAngle));
}

//void ACyclone::CameraYawInput(const FInputActionValue& Value)
//{
//	//SpringArm->AddRelativeRotation(FQuat(FVector(0, 0, 1), CameraRotationRate * Value * FApp::GetDeltaTime()));
//}
//
//void ACyclone::CameraPitchInput(const FInputActionValue& Value)
//{
//	//float DeltaAngle = CameraRotationRate * Value * FApp::GetDeltaTime();
//	//if (Camera->GetRelativeRotation().Pitch < CameraMinAngle && DeltaAngle > 0) return;
//	//if (Camera->GetRelativeRotation().Pitch > CameraMaxAngle && DeltaAngle < 0) return;
//	//
//	//Camera->AddLocalRotation(FQuat(FVector(0, 1, 0), DeltaAngle));
//}

FVector ACyclone::GetTargetLocation()
{
	return GetMesh()->GetComponentLocation() + GetMesh()->GetUpVector() * 20;
}

UPrimitiveComponent* ACyclone::GetRootComponent()
{
	return GetMesh();
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

	//FVector LookDirection = FMath::VInterpNormalRotationTo(
	//	MachineGunMesh->GetForwardVector(),
	//	(AimLocation - MachineGunMesh->GetComponentLocation()).GetUnsafeNormal(),
	//	DeltaTime,
	//	MachineGunRotationRate);
	//FVector BaseLookDirection = FVector::VectorPlaneProject(LookDirection, Body->GetUpVector());
	//FVector MachineGunLookDirection = FVector::VectorPlaneProject(LookDirection, MachineGunJointMesh->GetRightVector());

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

void ACyclone::GetDamage(float Damage)
{
	Health -= Damage;
	Health = Health >= 0 ? Health : 0;

	ValidateGameMode();
	if (GameMode)
	{
		GameMode->UpdateCarHealth(Health);
	}
}

void ACyclone::ValidateGameMode()
{
	if (GameMode == GetWorld()->GetAuthGameMode()) return;

	GameMode = Cast<ACarCombatMode>(GetWorld()->GetAuthGameMode());
}

void ACyclone::OnRadarEnter(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherComp->ComponentHasTag("Trigger")) return;
	if (!Cast<IDestroyable>(OtherActor)) return;

	ValidateGameMode();
	if (GameMode)
	{
		GameMode->ShowTarget(OtherActor);
	}
}

void ACyclone::OnRadarExit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherComp->ComponentHasTag("Trigger")) return;
	if (!Cast<IDestroyable>(OtherActor)) return;

	ValidateGameMode();
	if (GameMode)
	{
		GameMode->HideTarget(OtherActor);
	}
}