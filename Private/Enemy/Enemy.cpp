// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Enemy.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "MyProject/DebugMacros.h"
#include "Kismet/KismetSystemLibrary.h" // For drawing debug arrow

// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set Collisions
	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

	//Enable Overlap events
	GetMesh()->SetGenerateOverlapEvents(true);
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

void AEnemy::PlayHitReactMontage(const FName& SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && HitReactMontage)
	{
		AnimInstance->Montage_Play(HitReactMontage);
		AnimInstance->Montage_JumpToSection(SectionName, HitReactMontage);
	}
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemy::GetHit(const FVector& ImpactPoint)
{
	DRAW_SPHERE_COLOR(ImpactPoint, FColor::Orange);
	PlayHitReactMontage(FName("FromLeft"));

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
}

