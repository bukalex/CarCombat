// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CycloneWidget.generated.h"

/**
 * 
 */
UCLASS()
class CARCOMBAT_API UCycloneWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateCycloneHealth(float Value);
};
