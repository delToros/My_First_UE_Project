// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "HealthBarComponent.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API UHealthBarComponent : public UWidgetComponent
{
	GENERATED_BODY()
public:
	void SetHealthPercent(float Percent);

private:
	UPROPERTY() // This is needed so that HealthBarWidget would always be null pointer
	class UHealthBar* HealthBarWidget; // member variable instead of casting every time
	
};
