// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/BaseCharacter.h"
#include "DataObjects/Enums.h"
#include "Enemy.generated.h"

class UPawnSensingComponent;

UCLASS()
class MYPROJECT_API AEnemy : public ABaseCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void CheckPatrolTarget();

	void CheckCombatTarget();

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//Overriding GetHit fornm Interfaces
	virtual void GetHit_Implementation(const FVector& ImpactPoint) override;

	// Function Cpoied from take damage
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	//To destroy weapon of the enemy
	virtual void Destroyed() override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Handle enemies death
	virtual void Die() override;

	// Handle targets (combat and patrol)
	bool InTargetRange(AActor* Target, double Radius);
	
	// Refactoring move to target
	void MoveToTarget(AActor* Target);

	AActor* ChoosePatrolTarget();

	virtual void MainAttack() override;
	virtual void PlayAttacMontage() override;

	//For Pawn Sensing
	//we need UFunction because it is a delegate
	UFUNCTION()
	void PawnSeen(APawn* SeenPawn);


	UPROPERTY(BlueprintReadOnly)
	EDeathPose DeathPose = EDeathPose::EDP_Alive;


private:

	// -- Components 

	UPROPERTY(VisibleAnywhere)
	class UHealthBarComponent* HealthBarWidget;

	UPROPERTY(VisibleAnywhere)
	UPawnSensingComponent* PawnSensing;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AWeapon> WeaponClass;

	//so it does not have nullptr
	UPROPERTY()
	AActor* CombatTraget;

	UPROPERTY(EditAnywhere)
	double CombatRadius = 500.f;

	UPROPERTY(EditAnywhere)
	double AttackRadius = 150.f;

	// -- Navigation vars

	// Current patrol target
	UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
	AActor* PatrolTarget;

	UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
	TArray<AActor*> PatrolTargets;

	// not recommended to set as = to acceptance radius, better bigger (rounding)
	UPROPERTY(EditAnywhere)
	double PatrolRadius = 200.f;

	// Ai Controller
	class AAIController* EnemyController;

	UPROPERTY(EditAnywhere, Category = "AI Navigation")
	float WaitTimeMin = 5.f;

	UPROPERTY(EditAnywhere, Category = "AI Navigation")
	float WaittimeMax = 10.f;


	// -- Timer items
	FTimerHandle PatrolTimer;

	void PatrolTimerFinished();


	// -- enemy states
	EEnemyState EnemyState = EEnemyState::EES_Patrolling;
};
