// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/SecondItem.h"

// Sets default values
ASecondItem::ASecondItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASecondItem::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASecondItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

