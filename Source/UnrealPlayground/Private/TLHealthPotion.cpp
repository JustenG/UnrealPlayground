// Fill out your copyright notice in the Description page of Project Settings.


#include "TLHealthPotion.h"

#include "Components/SphereComponent.h"
#include "TLAttributeComponent.h"
#include "TLInteractionComponent.h"
#include "TLInteractionType.h"
#include "DrawDebugHelpers.h"


// Sets default values
ATLHealthPotion::ATLHealthPotion() : ATLGameItem()
{

}


void ATLHealthPotion::Interact_Implementation(APawn* InstigatorPawn, ETLInteractionType InteractionTypeUsed)
{
    if (!bIsActive)
    {
        //InteractionSuccessful = false;
        return;
    }

    if (InteractionTypeUsed == ETLInteractionType::EBT_CONTACT)
    {
        UTLAttributeComponent* AttributeComp = InstigatorPawn->GetComponentByClass<UTLAttributeComponent>();

        if (!AttributeComp->IsFullHealth())
        {
            AttributeComp->ApplyHealthChange(AttributeComp->GetHealthMax());
            UE_LOG(LogTemp, Log, TEXT("Player Healed"));

            UseGameItem();
            //InteractionSuccessful = true;
            return;
        }
    }
    //InteractionSuccessful = false;
}

