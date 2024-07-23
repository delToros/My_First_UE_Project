// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HealthBar.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API UHealthBar : public UUserWidget
{
	GENERATED_BODY()
public:

	// BindWidget - our HealthBar c++ var will be linked to the HealthBar Blueprint var
	// !! NAME HAS TO MATCH !!
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* HealthBar;

};
