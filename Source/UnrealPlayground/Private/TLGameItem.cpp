// Fill out your copyright notice in the Description page of Project Settings.


#include "TLGameItem.h"

// Sets default values
ATLGameItem::ATLGameItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATLGameItem::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATLGameItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATLGameItem::Interact_Implementation(APawn* InstigatorPawn)
{
}

