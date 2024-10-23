// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Widgets/CycloneWidget.h"
#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CarCombatMode.generated.h"

/**
 * 
 */
UCLASS()
class CARCOMBAT_API ACarCombatMode : public AGameModeBase
{
	GENERATED_BODY()
	
protected:
	UCycloneWidget* GameWidget;

	UPROPERTY(EditAnywhere)
	float Time = 120;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> GameWidgetClass;

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:
	void UpdateCarHealth(float Value);
	void ShowTarget(AActor* Target);
	void HideTarget(AActor* Target);
};
