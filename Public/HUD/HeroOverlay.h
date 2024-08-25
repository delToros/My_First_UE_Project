// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HeroOverlay.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API UHeroOverlay : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetHealthBarPercent(float Percent);

	void SetStaminahBarPercent(float Percent);

	void SetGold(int32 Gold);

	void SetSouls(int32 Souls);
	

private:

	UPROPERTY(meta = (BindWirget))
	class UProgressBar* HealthProgressBar;

	UPROPERTY(meta = (BindWirget))
	class UProgressBar* StaminaProgressBar;

	UPROPERTY(meta = (BindWirget))
	class UTextBlock* GoldText;

	UPROPERTY(meta = (BindWirget))
	class UTextBlock* SoulsText;
	
};
