// Fill out your copyright notice in the Description page of Project Settings.


#include "TLPickupItem.h"

#include "TLInteractionComponent.h"


// Sets default values
ATLPickupItem::ATLPickupItem()
{
	bIsActive = true;
	bRespawns = true;
	RespawnDelay = 10.0f;
	InteractionType = ETLInteractionType::EBT_CONTACT;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	RootComponent = MeshComp;

	SetReplicates(true);
}


void ATLPickupItem::Interact_Implementation(APawn* InstigatorPawn, ETLInteractionType InteractionTypeUsed)
{
	// Implement in derived classes
}


void ATLPickupItem::HideAndStartRespawnTimer()
{
	bIsActive = false;

	MeshComp->SetVisibility(false, true);

	GetWorldTimerManager().ClearTimer(TimerHandle_RespawnDelay);
	GetWorldTimerManager().SetTimer(TimerHandle_RespawnDelay, this, &ATLPickupItem::RespawnGameItem, RespawnDelay);
}


void ATLPickupItem::RespawnGameItem()
{
	bIsActive = true;

	MeshComp->SetVisibility(true, true);
}

