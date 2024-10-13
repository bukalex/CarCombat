// Fill out your copyright notice in the Description page of Project Settings.


#include "CarCombatMode.h"

void ACarCombatMode::BeginPlay()
{
	Super::BeginPlay();

	if (!GameWidgetClass) return;
	
	GameWidget = Cast<UCycloneWidget>(CreateWidget(GetWorld(), GameWidgetClass));
	GameWidget->AddToViewport();
}

void ACarCombatMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Time -= DeltaTime;
	Time = Time >= 0 ? Time : 0;

	if (!GameWidget) return;

	GameWidget->UpdateTimer(int(Time));
}

void ACarCombatMode::UpdateCarHealth(float Value)
{
	if (!GameWidget) return;

	GameWidget->UpdateCycloneHealth(Value);
}