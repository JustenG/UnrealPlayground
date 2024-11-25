// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TLPickupItem.h"
#include "TLInteractionType.h"
#include "TLPickupItem_HealthPotion.generated.h"


UCLASS()
class UNREALPLAYGROUND_API ATLPickupItem_HealthPotion : public ATLPickupItem
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATLPickupItem_HealthPotion();

// Custom Code
protected:

	UPROPERTY(EditDefaultsOnly, Category = "Credits")
	int32 CreditsCost;

public:

	virtual void Interact_Implementation(APawn* InstigatorPawn, ETLInteractionType InteractionTypeUsed) override;

	virtual FText GetInteractText_Implementation(APawn* InstigatorPawn) override;
};
