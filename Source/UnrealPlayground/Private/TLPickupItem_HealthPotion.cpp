// Fill out your copyright notice in the Description page of Project Settings.


#include "TLPickupItem_HealthPotion.h"

#include "Components/SphereComponent.h"
#include "TLAttributeComponent.h"
#include "TLInteractionComponent.h"
#include "TLInteractionType.h"
#include "DrawDebugHelpers.h"
#include "TLPlayerState.h"


// Sets default values
ATLPickupItem_HealthPotion::ATLPickupItem_HealthPotion() : ATLPickupItem()
{
	CreditsCost = 100;
}


void ATLPickupItem_HealthPotion::Interact_Implementation(APawn* InstigatorPawn, ETLInteractionType InteractionTypeUsed)
{
	if (!bIsActive)
	{
		return;
	}

	if (InteractionTypeUsed == ETLInteractionType::EBT_NEARBY)
	{
		ATLPlayerState* PlayerState = InstigatorPawn->GetPlayerState<ATLPlayerState>();
		if (PlayerState && PlayerState->RemoveCredits(CreditsCost))
		{	
			UTLAttributeComponent* AttributeComp = InstigatorPawn->GetComponentByClass<UTLAttributeComponent>();
	
			if (!AttributeComp->IsFullHealth())
			{
				AttributeComp->ApplyHealthChange(this, AttributeComp->GetHealthMax());
				UE_LOG(LogTemp, Log, TEXT("Player Healed"));
	
				HideAndStartRespawnTimer();
				return;
			}
		}
	}
}

