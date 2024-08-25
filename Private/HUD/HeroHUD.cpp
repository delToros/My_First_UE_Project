// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/HeroHUD.h"
#include "HUD/HeroOverlay.h"

void AHeroHUD::BeginPlay()
{

	Super::BeginPlay();

	UWorld* World = GetWorld();
	if (World)
	{
		APlayerController* Controller = World->GetFirstPlayerController();
		if (Controller && HeroOverlayClass)
		{
			UHeroOverlay* HeroOverlay = CreateWidget<UHeroOverlay>(Controller, HeroOverlayClass);
			HeroOverlay->AddToViewport();

		}
		}
	}

	
