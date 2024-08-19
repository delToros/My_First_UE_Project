// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Item.h"
#include "Weapon.generated.h"

class USoundBase;
class UBoxComponent;

/**
 * 
 */
UCLASS()
class MYPROJECT_API AWeapon : public AItem
{
	GENERATED_BODY()

public:
	AWeapon(); // This is constructor (why - becuase it same name as the class. And because it is constructor we do not need return type

	void Equip(USceneComponent* InParent, FName InSocketName, AActor* NewOwner, APawn* NewInstigator);

	void DeactivateEmbers();

	void DissableSphereCollision();

	void PlayEquipSound();

	void AttachMeshToSocket(USceneComponent* InParent, const FName& InSocketName);

	FORCEINLINE UBoxComponent* GetWeaponBox() const { return WeaponBox;  }

	TArray<AActor*> IgnoreActors;

protected:

	virtual void BeginPlay() override;

	// not override = it is new function
	// not virtual = we can make it later if we need to
	UFUNCTION()
	void OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	bool ActorSameType(AActor* OtherActor);

	void ExecuteGetHit(FHitResult& BoxHit);
	
	UFUNCTION(BlueprintImplementableEvent) // With this event we do need to create function definition
	void CreateFields(const FVector& FieldLocation);

private:
	
	void BoxTrace(FHitResult& BoxHit);

	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	FVector BoxTraceExtent = FVector(5.f);

	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	bool bShowBoxDebug = false;

	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	USoundBase* EquipSound;

	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
	UBoxComponent* WeaponBox;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* BoxTraceStart;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* BoxTraceEnd;

	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	float Damage = 20.f;
};
