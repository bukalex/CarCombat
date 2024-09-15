// Fill out your copyright notice in the Description page of Project Settings.


#include "TurretBase.h"

// Sets default values
ATurretBase::ATurretBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>("Base Mesh");
	RootComponent = BaseMesh;

	AttackZone = CreateDefaultSubobject<USphereComponent>("Attack Zone");
	AttackZone->SetupAttachment(BaseMesh);
	AttackZone->OnComponentBeginOverlap.AddDynamic(this, &ATurretBase::OnAttackZoneEnter);
	AttackZone->OnComponentEndOverlap.AddDynamic(this, &ATurretBase::OnAttackZoneExit);
}

// Called when the game starts or when spawned
void ATurretBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATurretBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Aim(DeltaTime);
	CheckIfLockedOnTarget();
}

void ATurretBase::Aim(float DeltaTime)
{
	if (!Target) return;
	if (!GetWorld()) return;

	bTargetWithinRotationLimit = IsTargetWithinRotationLimit();
	bTargetVisible = IsTargetVisible();

	if (!bTargetWithinRotationLimit || !bTargetVisible) AimingLineColor = FColor::Red;
	else AimingLineColor = bLockedOn ? FColor::Blue : FColor::Green;

	DrawDebugLine(GetWorld(), GetFiringComponent()->GetComponentLocation(), Target->GetTargetLocation(), AimingLineColor);
}

void ATurretBase::CheckIfLockedOnTarget()
{
	if (!Target) return;

	bLockedOn = (Target->GetAngleToTarget(Target->GetTargetDirection(GetFiringComponent()->GetComponentLocation()), GetFiringComponent()->GetForwardVector()) < AimingPrecisionAngle);
}

bool ATurretBase::IsTargetWithinRotationLimit()
{
	if (!Target) return false;
	return true;
}

bool ATurretBase::IsTargetVisible()
{
	if (!Target) return false;
	if (!GetWorld()) return false;

	FHitResult Hit;
	FCollisionQueryParams CollisionQueryParams = FCollisionQueryParams();
	CollisionQueryParams.AddIgnoredActor(this);

	bool bActorHit = GetWorld()->LineTraceSingleByChannel(Hit, GetFiringComponent()->GetComponentLocation(), Target->GetTargetLocation(), ECollisionChannel::ECC_Pawn, CollisionQueryParams);
	if (!bActorHit) return false;
	if (!Cast<ITurretTargetable>(Hit.GetActor())) return false;

	return true;
}

USceneComponent* ATurretBase::GetFiringComponent()
{
	return BaseMesh;
}

void ATurretBase::OnAttackZoneEnter(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!Cast<ITurretTargetable>(OtherActor)) return;

	Target = Cast<ITurretTargetable>(OtherActor);
}

void ATurretBase::OnAttackZoneExit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!Target) return;
	if (Target != Cast<ITurretTargetable>(OtherActor)) return;
	
	Target = nullptr;
}