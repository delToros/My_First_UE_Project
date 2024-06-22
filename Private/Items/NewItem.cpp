// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/NewItem.h"

// Sets default values
ANewItem::ANewItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ANewItem::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ANewItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

