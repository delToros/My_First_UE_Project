// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Enemy.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MyProject/DebugMacros.h"
#include "Kismet/KismetSystemLibrary.h" // For drawing debug arrow
#include "Kismet/GameplayStatics.h" // For sounds
#include "Components/AttributeComponent.h" // for attributes
#include "HUD/HealthBarComponent.h" // For WidgetComponent - Changed to UHealthBarComponent
#include "AIController.h" // For navigation
#include "Perception/PawnSensingComponent.h" 

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

	// add attributes component
	Attributes = CreateDefaultSubobject<UAttributeComponent>(TEXT("Attributes"));

	// add health bar widget
	HealthBarWidget = CreateDefaultSubobject<UHealthBarComponent>(TEXT("Health Bar"));
	HealthBarWidget->SetupAttachment(GetRootComponent());

	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	// PawnSensingComponent
	PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensing")); // we do need to attach it
	PawnSensing->SightRadius = 4000.f;
	PawnSensing->SetPeripheralVisionAngle(45.f);
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	HealthBarWidget->SetHealthPercent(Attributes->GetHealthPercent());
	
	// at the beginplay health bar is hidden
	if (HealthBarWidget)
	{
		HealthBarWidget->SetVisibility(false);
	}

	EnemyController = Cast<AAIController>(GetController());

	// Strat navigation
	MoveToTarget(PatrolTarget);

	if (PawnSensing)
	{
		PawnSensing->OnSeePawn.AddDynamic(this, &AEnemy::PawnSeen);
	}

}

void AEnemy::Die()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && DeathMontage)
	{
		AnimInstance->Montage_Play(DeathMontage);

		const int32 Selection = FMath::RandRange(0, 4);
		FName SectionName = FName();
		switch (Selection)
		{
		case 0:
			SectionName = FName("Death1");
			DeathPose = EDeathPose::EDP_Death1;
			break;
		case 1:
			SectionName = FName("Death2");
			DeathPose = EDeathPose::EDP_Death2;
			break;
		case 2:
			SectionName = FName("Death3");
			DeathPose = EDeathPose::EDP_Death3;
			break;
		case 3:
			SectionName = FName("Death4");
			DeathPose = EDeathPose::EDP_Death4;
			break;
		case 4:
			SectionName = FName("Death5");
			DeathPose = EDeathPose::EDP_Death5;
			break;
		default:
			break;
		}

		AnimInstance->Montage_JumpToSection(SectionName, DeathMontage);

		if (HealthBarWidget)
		{
			HealthBarWidget->SetVisibility(false);
		}
	}

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	SetLifeSpan(3.f);

}

bool AEnemy::InTargetRange(AActor* Target, double Radius)
{
	//part of refactor
	if (Target == nullptr) return false;
	const double DistanceToTarget = (Target->GetActorLocation() - GetActorLocation()).Size();
	// For debug purposes
	DRAW_SPHERE_SINGLEFRAME(GetActorLocation());
	DRAW_SPHERE_SINGLEFRAME(Target->GetActorLocation());
	//

	return DistanceToTarget <= Radius;
}

void AEnemy::MoveToTarget(AActor* Target)
{
	if (EnemyController == nullptr || Target == nullptr) return;
	FAIMoveRequest MoveRequest;
	MoveRequest.SetGoalActor(Target);
	MoveRequest.SetAcceptanceRadius(15.f);
	EnemyController->MoveTo(MoveRequest);

}

AActor* AEnemy::ChoosePatrolTarget()
{
	TArray<AActor*> ValidTargets;
	for (AActor* Target : PatrolTargets)
	{
		if (Target != PatrolTarget)
		{
			ValidTargets.AddUnique(Target);
		}
	}

	const int32 NumPatrolTargets = ValidTargets.Num();
	if (NumPatrolTargets > 0)
	{
		const int32 TargetSelection = FMath::RandRange(0, NumPatrolTargets - 1);
		return ValidTargets[TargetSelection];
	}

	return nullptr;
}

void AEnemy::PawnSeen(APawn* SeenPawn)
{
	UE_LOG(LogTemp, Warning, TEXT("Pawn Seen!"));
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

void AEnemy::PatrolTimerFinished()
{
	MoveToTarget(PatrolTarget);
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	CheckCombatTarget();

	
	CheckPatrolTarget();

}

void AEnemy::CheckPatrolTarget()
{
	if (InTargetRange(PatrolTarget, PatrolRadius))
	{
		PatrolTarget = ChoosePatrolTarget();
		const float RandomWaitTime = FMath::RandRange(WaitTimeMin, WaittimeMax);

		GetWorldTimerManager().SetTimer(
			PatrolTimer,
			this,
			&AEnemy::PatrolTimerFinished,
			RandomWaitTime
		);
	}
}

void AEnemy::CheckCombatTarget()
{
	if (!InTargetRange(CombatTraget, CombatRadius))// ! means NOT
	{
		CombatTraget = nullptr;
		if (HealthBarWidget)
		{
			HealthBarWidget->SetVisibility(false);
		}
	}
}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemy::GetHit_Implementation(const FVector& ImpactPoint)
{
	//DRAW_SPHERE_COLOR(ImpactPoint, FColor::Orange);

	//if hit - show healthbar
	if (HealthBarWidget)
	{
		HealthBarWidget->SetVisibility(true);
	}

	// Check if has health after hit, if not - play death
	if (Attributes && Attributes->isAlive())
	{
		DirectionalHitReact(ImpactPoint);
	}
	else
	{
		Die();
	}
	

	if (HitSound)
	{
		UGameplayStatics::PlaySoundAtLocation(
			this,
			HitSound,
			ImpactPoint
		);
	}

	if (HitParticles && GetWorld()) // Get World - is just being extra safe, not neccecery
	{
		UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			HitParticles,
			ImpactPoint
		);
	}

}

void AEnemy::DirectionalHitReact(const FVector& ImpactPoint)
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

float AEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (Attributes && HealthBarWidget)
	{
		Attributes->ReceiveDamage(DamageAmount);
		HealthBarWidget->SetHealthPercent(Attributes->GetHealthPercent());
	}

	CombatTraget = EventInstigator->GetPawn();
	return DamageAmount;
}

