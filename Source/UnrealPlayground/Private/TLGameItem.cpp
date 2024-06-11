// Fill out your copyright notice in the Description page of Project Settings.


#include "TLGameItem.h"

#include "TLInteractionComponent.h"

// Sets default values
ATLGameItem::ATLGameItem()
{
	bIsActive = true;
	bRespawns = true;
	RespawnDelay = 10.0f;
	InteractionType = ETLInteractionType::EBT_CONTACT;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	RootComponent = MeshComp;
}

void ATLGameItem::Interact_Implementation(APawn* InstigatorPawn, ETLInteractionType InteractionTypeUsed)
{
	// Implement in derived classes
}

void ATLGameItem::UseGameItem()
{
	bIsActive = false;

	MeshComp->SetVisibility(false, true);

	GetWorldTimerManager().ClearTimer(TimerHandle_RespawnDelay);
	GetWorldTimerManager().SetTimer(TimerHandle_RespawnDelay, this, &ATLGameItem::RespawnGameItem, RespawnDelay);
}

void ATLGameItem::RespawnGameItem()
{
	bIsActive = true;

	MeshComp->SetVisibility(true, true);
}

