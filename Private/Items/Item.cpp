// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Item.h"
#include "DrawDebugHelpers.h"
#include "MyProject/DebugMacros.h"
#include "Components/SphereComponent.h"
#include "Interfaces/PickupInterface.h"
#include "Characters/Hero.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AItem::AItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Creating new static Mesh Component. to do that -> creating default subobject
	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMeshComponent"));

	//Disable collisions for all items
	ItemMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	ItemMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	RootComponent = ItemMesh;

	// Creating sphere for collisions
	SphereNewCom = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereNewCom->SetupAttachment(GetRootComponent());

	//Niagara Component
	ItemEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Embers"));
	ItemEffect->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();

	int32 AvgInt = Avg<int32>(1, 3);
	UE_LOG(LogTemp, Warning, TEXT("Avg of 1 and 3: %d"), AvgInt);


	float AvgFloat = Avg<float>(3.45f, 7.86f);
	UE_LOG(LogTemp, Warning, TEXT("Avg of 1 and 3: %f"), AvgFloat);

	SphereNewCom->OnComponentBeginOverlap.AddDynamic(this, &AItem::OnSphereOverlap);

	SphereNewCom->OnComponentEndOverlap.AddDynamic(this, &AItem::EndSphereOverlap);
	
}

float AItem::TransformedSined()
{
	return Amplitude * FMath::Sin(RunningTime * TimeConstant);
}

float AItem::TransformedCosine()
{
	return Amplitude * FMath::Cos(RunningTime * TimeConstant);
}

void AItem::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	const FString AotherActorName = OtherActor->GetName();
	//if (GEngine)
	//{
	//	GEngine->AddOnScreenDebugMessage(1, 30.f, FColor::Red, AotherActorName);
	//}

	// 1. Make a local var
	IPickupInterface* PickupInterface = Cast<IPickupInterface>(OtherActor);

	// 2. Check Hero Character if it is not a null pointer
	if (PickupInterface)
	{
		// Set the overlapping itemn on the character
		PickupInterface->SetOverlappingItem(this);
	}
}

void AItem::EndSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	//const FString MessTxt = "Fucking Fuck";
	//if (GEngine)
	//{
	//	GEngine->AddOnScreenDebugMessage(1, 30.f, FColor::Blue, MessTxt);
	//}

	// 1. Make a local var
	IPickupInterface* PickupInterface = Cast<IPickupInterface>(OtherActor);

	// 2. Check Hero Character if it is not a null pointer
	if (PickupInterface)
	{
		// Set the overlapping itemn on the character
		PickupInterface->SetOverlappingItem(nullptr);
	}
}

void AItem::SpawnPickupSystem()
{
	if (PickupEffect)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			this,
			PickupEffect,
			GetActorLocation());
	}
}

void AItem::SpawnPickupSound()
{
	if (PickupSound)
	{
		UGameplayStatics::SpawnSoundAtLocation(
			this,
			PickupSound,
			GetActorLocation()
		);
	}
}

// Called every frame
void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	RunningTime += DeltaTime;

	if (ItemState == EItemState::EIS_Hovering)
	{
		AddActorWorldOffset(FVector(0.f, 0.f, TransformedSined()));
	}


}

