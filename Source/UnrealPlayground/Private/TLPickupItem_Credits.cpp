// Fill out your copyright notice in the Description page of Project Settings.


#include "TLPickupItem_Credits.h"
#include "TLPlayerState.h"


ATLPickupItem_Credits::ATLPickupItem_Credits()
{
	CreditsGranted = 10;
}


void ATLPickupItem_Credits::Interact_Implementation(APawn* InstigatorPawn, ETLInteractionType InteractionTypeUsed)
{
	if (!bIsActive)
	{
		return;
	}

	if (InteractionTypeUsed == ETLInteractionType::EBT_CONTACT)
	{
		ATLPlayerState* PlayerState = InstigatorPawn->GetPlayerState<ATLPlayerState>();
		if (PlayerState)
		{
			PlayerState->AddCredits(CreditsGranted);
			HideAndStartRespawnTimer();
		}
	}
}
