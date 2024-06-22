// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/HeroAnimInstance.h"
#include "Characters/Hero.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UHeroAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	HeroCharacter = Cast<AHero>(TryGetPawnOwner()); // This cast is UE specific
	if (HeroCharacter) // We are checking because Cast can return null
	{
		HeroCharacterMovement = HeroCharacter->GetCharacterMovement();
	}

}

void UHeroAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (HeroCharacterMovement)
	{
		GroundSpeed = UKismetMathLibrary::VSizeXY(HeroCharacterMovement->Velocity);

		IsFalling = HeroCharacterMovement->IsFalling();

		CharacterState = HeroCharacter->GetCharacterState();
	}
}
