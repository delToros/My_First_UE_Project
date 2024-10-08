// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/HeroOverlay.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UHeroOverlay::SetHealthBarPercent(float Percent)
{
	if (HealthProgressBar)
	{
		UE_LOG(LogTemp, Warning, TEXT("HealthProgressBar1 is valid. Setting percent to: %f"), Percent);
		HealthProgressBar->SetPercent(Percent);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("HealthProgressBar1 is nullptr."));
	}
}

void UHeroOverlay::SetStaminahBarPercent(float Percent)
{
	if (StaminaProgressBar)
	{
		StaminaProgressBar->SetPercent(Percent);
	}
}

void UHeroOverlay::SetGold(int32 Gold)
{
	if (GoldText)
	{
		
		GoldText->SetText(FText::FromString(FString::Printf(TEXT("%d"), Gold)));
	}
}


void UHeroOverlay::SetSouls(int32 Souls)
{
	if (SoulsText)
	{
		SoulsText->SetText(FText::FromString(FString::Printf(TEXT("%d"), Souls)));
	}
}
