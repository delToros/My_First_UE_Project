// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "HeroHUD.generated.h"



/**
 * 
 */
UCLASS()
class MYPROJECT_API AHeroHUD : public AHUD
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;


private:

	UPROPERTY(EditDefaultsOnly, Category = Hero)
	TSubclassOf<class UHeroOverlay> HeroOverlayClass;

	UPROPERTY()
	UHeroOverlay* HeroOverlay;

public:
	FORCEINLINE UHeroOverlay* GetHeroOverlay() const { return HeroOverlay;  }
};
