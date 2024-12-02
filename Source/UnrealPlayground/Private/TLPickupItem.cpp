// Fill out your copyright notice in the Description page of Project Settings.


#include "TLPickupItem.h"

#include "TLInteractionComponent.h"
#include "Net/UnrealNetwork.h"


void ATLPickupItem::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATLPickupItem, bIsActive);
}


// Sets default values
ATLPickupItem::ATLPickupItem()
{
	bIsActive = true;
	bRespawns = true;
	RespawnDelay = 10.0f;
	InteractionType = ETLInteractionType::EBT_CONTACT;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	RootComponent = MeshComp;

	bReplicates = true;
	//SetReplicates(true);
}


bool ATLPickupItem::GetIsActive()
{
	return bIsActive;
}


void ATLPickupItem::Interact_Implementation(APawn* InstigatorPawn, ETLInteractionType InteractionTypeUsed)
{
	// Implement in derived classes
}


FText ATLPickupItem::GetInteractText_Implementation(APawn* InstigatorPawn)
{
	// Implement in derived classes
	return FText::GetEmpty();
}


void ATLPickupItem::OnRep_IsActive()
{
	MeshComp->SetVisibility(bIsActive, true);
}


void ATLPickupItem::HideAndStartRespawnTimer()
{
	bIsActive = false;
	OnRep_IsActive();

	GetWorldTimerManager().ClearTimer(TimerHandle_RespawnDelay);
	GetWorldTimerManager().SetTimer(TimerHandle_RespawnDelay, this, &ATLPickupItem::RespawnGameItem, RespawnDelay);
}


void ATLPickupItem::RespawnGameItem()
{
	bIsActive = true;
	OnRep_IsActive();
}

