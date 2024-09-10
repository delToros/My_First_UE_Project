// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "DataObjects/Enums.h"
#include "HeroAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API UHeroAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	//This is C++ version of Event Blueprint Initialize Animation
	virtual void NativeInitializeAnimation() override;

	// This is C++ version of Event Blueprint Update Animation
	virtual void NativeUpdateAnimation(float DeltaTime) override;
	
	UPROPERTY(BlueprintReadOnly)
	class AHero* HeroCharacter; // This is forwards declare

	UPROPERTY(BlueprintReadOnly, Category = Movement)
	class UCharacterMovementComponent* HeroCharacterMovement;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
	float GroundSpeed;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
	bool IsFalling;

	UPROPERTY(BlueprintReadOnly, Category = "Movement | Character State")
	ECharacterState CharacterState;

	UPROPERTY(BlueprintReadOnly, Category = "Action State")
	EActionState ActionState;

	UPROPERTY(BlueprintReadOnly, Category = "Action State")
	TEnumAsByte<EDeathPose> DeathPose;
};
