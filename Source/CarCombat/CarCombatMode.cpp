// Fill out your copyright notice in the Description page of Project Settings.


#include "CarCombatMode.h"

void ACarCombatMode::BeginPlay()
{
	if (!GameWidgetClass) return;
	
	GameWidget = Cast<UCycloneWidget>(CreateWidget(GetWorld(), GameWidgetClass));
	GameWidget->AddToViewport();
}

void ACarCombatMode::UpdateCarHealth(float Value)
{
	if (!GameWidget) return;

	GameWidget->UpdateCycloneHealth(Value);
}