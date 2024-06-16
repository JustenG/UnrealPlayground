// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TLPickupItem.h"
#include "TLPickupItem_Credits.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPLAYGROUND_API ATLPickupItem_Credits : public ATLPickupItem
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATLPickupItem_Credits();

	// Custom Code
protected:

	UPROPERTY(EditDefaultsOnly, Category = "Credits")
	int32 CreditsGranted;

public:

	virtual void Interact_Implementation(APawn* InstigatorPawn, ETLInteractionType InteractionTypeUsed) override;
};
