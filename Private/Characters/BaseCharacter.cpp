// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/BaseCharacter.h"
#include "Components/BoxComponent.h"
#include "Items/Weapons/Weapon.h"
#include "Components/AttributeComponent.h" // for attributes


// Sets default values
ABaseCharacter::ABaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// add attributes component
	Attributes = CreateDefaultSubobject<UAttributeComponent>(TEXT("Attributes"));

}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABaseCharacter::MainAttack()
{
}

void ABaseCharacter::Die()
{
}

void ABaseCharacter::PlayAttacMontage()
{
}

void ABaseCharacter::PlayHitReactMontage(const FName& SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && HitReactMontage)
	{
		AnimInstance->Montage_Play(HitReactMontage);
		AnimInstance->Montage_JumpToSection(SectionName, HitReactMontage);
	}
}

void ABaseCharacter::DirectionalHitReact(const FVector& ImpactPoint)
{
	// Calculating DOT PRODUCT
	// 1. setup 2 vectors
	const FVector Forward = GetActorForwardVector();
	// Added later - so that debug arrows point in the same z location
	const FVector ImactLowered(ImpactPoint.X, ImpactPoint.Y, GetActorLocation().Z);
	const FVector ToHit = (ImactLowered - GetActorLocation()).GetSafeNormal(); //GetSafeNormal - normalize vectors. + Here we can use Impact Point instead of lowered

	// 2. get angle between these 2 vectors
	// 2.1 Find dot product
	const double CosTheta = FVector::DotProduct(Forward, ToHit);
	// Explanation
	// Forward * ToHit = |Forward||ToHit| * cos(theta)
	// |Forward| = 1, |ToHit| = 1 (normalized), so Forward * ToHit = cos(theta)

	// 2.2 Get arc cos
	double Theta = FMath::Acos(CosTheta); // Take the inverse cosine (arc-cosine) of cos(theta) to get theta
	// Result here is radians

	// 2.3 Convert from radians to degrees
	Theta = FMath::RadiansToDegrees(Theta);

	// 4. Calculate Cross Product and draw lkine
	const FVector CrossProduct = FVector::CrossProduct(Forward, ToHit);

	// 4.1 Checke, if Cross Product points down, Theta should be negative
	if (CrossProduct.Z < 0)
	{
		Theta *= -1.f;
	}
	/*
	UKismetSystemLibrary::DrawDebugArrow(
		this,
		GetActorLocation(),
		GetActorLocation() + CrossProduct * 100.f,
		5.f,
		FColor::Blue,
		5.f
	);
	*/

	// Default value instead of last else
	FName Section("FromBack");

	if (Theta >= -45.f && Theta < 45.f)
	{
		Section = FName("FromFront");
	}
	else if (Theta >= -135.f && Theta < -45.f)
	{
		Section = FName("FromLeft");
	}
	else if (Theta >= 45.f && Theta < 135.f)
	{
		Section = FName("FromRight");
	}

	PlayHitReactMontage(Section);

	/*
	GEngine->AddOnScreenDebugMessage(2, 5.f, FColor::Red, Section.ToString());

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Green, FString::Printf(TEXT("Thetd: %f"), Theta));
	}

	// Arrow from enemy location straight out
	UKismetSystemLibrary::DrawDebugArrow(
		this,
		GetActorLocation(),
		GetActorLocation() + Forward * 60.f,
		5.f,
		FColor::Red,
		5.f
	);

	// Arrow from enemy location to the hit location
	UKismetSystemLibrary::DrawDebugArrow(
		this,
		GetActorLocation(),
		GetActorLocation() + ToHit * 60.f,
		5.f,
		FColor::Green,
		5.f
	);
	*/
}

bool ABaseCharacter::CanAttack()
{
	return false;
}

void ABaseCharacter::AttackEnd()
{
}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseCharacter::SetWeaponCollisionEnabled(ECollisionEnabled::Type CollisionEnabled)
{
	if (EquipWeapon && EquipWeapon->GetWeaponBox())
	{
		EquipWeapon->GetWeaponBox()->SetCollisionEnabled(CollisionEnabled);

		EquipWeapon->IgnoreActors.Empty();
	}
}

