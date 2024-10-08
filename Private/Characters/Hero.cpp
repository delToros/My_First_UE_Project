// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Hero.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GroomComponent.h"
#include "Items/Item.h"
#include "Items/Weapons/Weapon.h"
#include "DataObjects/Enums.h"
#include "Animation/AnimMontage.h"
#include "Components/StaticMeshComponent.h"

#include "HUD/HeroHUD.h"
#include "HUD/HeroOverlay.h"
#include "Components/AttributeComponent.h"

#include "Items/Soul.h"
#include "Items/Treasure.h"

// Sets default values
AHero::AHero()
{
	// Set this character to call Tick() every frame. You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Orient rotation to movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 360.f, 0.f);

	// Set collisions to overlap with enemys' weapon
	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Overlap);
	GetMesh()->SetGenerateOverlapEvents(true);


	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(GetRootComponent());
	SpringArm->TargetArmLength = 300.f;

	// Makes the Spring arm rotate with the Look() function
	SpringArm->bUsePawnControlRotation = true;

	ViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ViewCamera"));
	ViewCamera->SetupAttachment(SpringArm);

	// Rotate the camera above bird	
	SpringArm->AddRelativeRotation(FRotator(-20.f, 0.f, 0.f));

	// Stops the camera from rotating with the controller. Only the spring arm.
	ViewCamera->bUsePawnControlRotation = false;

	Hair = CreateDefaultSubobject<UGroomComponent>(TEXT("Hair"));
	Hair->SetupAttachment(GetMesh());
	Hair->AttachmentName = FString("head");

	Eyebrows = CreateDefaultSubobject<UGroomComponent>(TEXT("Eyebrows"));
	Eyebrows->SetupAttachment(GetMesh());
	Eyebrows->AttachmentName = FString("head");
}

// Called when the game starts or when spawned
void AHero::BeginPlay()
{
	Super::BeginPlay();

	Tags.Add(FName("MainHero"));

	if (APlayerController* HeroController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(HeroController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(HeroMappingContext, 0);
		}
	}
	InitializeHeroOverlay();

}

void AHero::InitializeHeroOverlay()
{
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		UE_LOG(LogTemp, Warning, TEXT("Player controller"));
		AHeroHUD* HeroHUD = Cast<AHeroHUD>(PlayerController->GetHUD());
		if (HeroHUD)
		{
			HeroOverlay = HeroHUD->GetHeroOverlay();
			if (HeroOverlay && Attributes)
			{
				HeroOverlay->SetHealthBarPercent(Attributes->GetHealthPercent());

				HeroOverlay->SetStaminahBarPercent(1.f);
				HeroOverlay->SetGold(0);
				HeroOverlay->SetSouls(0);
			}


		}
	}
}

void AHero::Move(const FInputActionValue& Value)
{
	if (ActionState != EActionState::EAS_Unoccupied) return;

	const FVector2D MovementVector = Value.Get<FVector2D>();

	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	AddMovementInput(ForwardDirection, MovementVector.Y);

	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	AddMovementInput(RightDirection, MovementVector.X);

}

void AHero::Look(const FInputActionValue& Value)
{
	const FVector2D LookAxisVector = Value.Get<FVector2D>();

	AddControllerPitchInput(LookAxisVector.Y);
	AddControllerYawInput(LookAxisVector.X);

}

void AHero::EKeyPressed()
{
	AWeapon* OverlappingWeapon = Cast<AWeapon>(OverlappingItem);

	// If Overlapping item is not null pointer
	if (OverlappingWeapon)
	{
		EquipWeapon(OverlappingWeapon);
	}
	else
	{
		if (CanDisarm())
		{
			Disarm();
		}
		else if (CanArm())
		{
			Arm();
		}
	}

}

void AHero::MainAttack()
{
	Super::MainAttack();
	if (CanAttack())
	{
		PlayAttackMontage();
		ActionState = EActionState::EAS_Attacking;
	}
	
}

bool AHero::CanAttack()
{
	return ActionState == EActionState::EAS_Unoccupied &&
		CharacterState != ECharacterState::ECS_Unequipped;
}

bool AHero::CanArm()
{
	return ActionState == EActionState::EAS_Unoccupied && 
		CharacterState == ECharacterState::ECS_Unequipped &&
		EquippedWeapon;
}

bool AHero::CanDisarm()
{
	return ActionState == EActionState::EAS_Unoccupied &&
		CharacterState != ECharacterState::ECS_Unequipped;
}

void AHero::Disarm()
{
	PlayEquipMontage(FName("Disarm"));
	CharacterState = ECharacterState::ECS_Unequipped;
	ActionState = EActionState::EAS_EquippingWeapon;
}

void AHero::AttachWeaponToBack()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->AttachMeshToSocket(GetMesh(), FName("spine_05Socket"));
	}
}

void AHero::AttachWeaponToHand()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->AttachMeshToSocket(GetMesh(), FName("RightHandSocket"));
	}
}

void AHero::FinishEquipping()
{
	ActionState = EActionState::EAS_Unoccupied;
}

void AHero::HitReactEnd()
{
	ActionState = EActionState::EAS_Unoccupied;
}

void AHero::Arm()
{
	PlayEquipMontage(FName("Equip"));
	CharacterState = ECharacterState::ECS_EquippedOneHandedWeapon;
	ActionState = EActionState::EAS_EquippingWeapon;
}

void AHero::PlayEquipMontage(FName SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && EquipMontage)
	{
		AnimInstance->Montage_Play(EquipMontage);
		AnimInstance->Montage_JumpToSection(SectionName, EquipMontage);
	}
}

void AHero::Die()
{
	Super::Die();
	
	ActionState = EActionState::EAS_Dead;

	DisableMeshCollision();

}

void AHero::EquipWeapon(AWeapon* Weapon)
{
	// Call the equip function from Weapons.cpp
	Weapon->Equip(GetMesh(), FName("RightHandSocket"), this, this);
	CharacterState = ECharacterState::ECS_EquippedOneHandedWeapon;
	OverlappingItem = nullptr; // otherwise it will always be true and not enter in else statement.
	EquippedWeapon = Weapon;
}

void AHero::AttackEnd()
{
	ActionState = EActionState::EAS_Unoccupied;
}

//void AHero::Jump()
//{
//	Super::Jump();
//}

// Called to bind functionality to input
void AHero::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);


	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AHero::Move);

		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AHero::Look);

		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AHero::Jump);

		EnhancedInputComponent->BindAction(EKeyAction, ETriggerEvent::Triggered, this, &AHero::EKeyPressed);

		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &AHero::MainAttack);
	}
}

void AHero::Jump()
{
	if (IsUnoccupied())
	{
		Super::Jump();
	}
	

}

bool AHero::IsUnoccupied()
{
	return ActionState == EActionState::EAS_Unoccupied;
}

float AHero::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	HandleDamage(DamageAmount);
	SetHUDHealth();
	return DamageAmount;
}

void AHero::SetHUDHealth()
{
	if (HeroOverlay && Attributes)
	{
		HeroOverlay->SetHealthBarPercent(Attributes->GetHealthPercent());
	}
}

void AHero::GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter)
{
	Super::GetHit_Implementation(ImpactPoint, Hitter);

	SetWeaponCollisionEnabled(ECollisionEnabled::NoCollision);

	if (Attributes && Attributes->GetHealthPercent() > 0.f)
	{
		ActionState = EActionState::EAS_HitReaction;
	}

	
}

void AHero::SetOverlappingItem(AItem* Item)
{
	OverlappingItem = Item;
}

void AHero::AddSouls(ASoul* Soul)
{
	if (Attributes && HeroOverlay)
	{
		Attributes->AddSouls(Soul->GetSouls());
		HeroOverlay->SetSouls(Attributes->GetSouls());
	}
}

void AHero::AddGold(ATreasure* Treasure)
{
	if (Attributes && HeroOverlay)
	{
		Attributes->AddGold(Treasure->GetGold());
		HeroOverlay->SetGold(Attributes->GetGold());
	}
}



