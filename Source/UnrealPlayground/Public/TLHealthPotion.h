// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TLGameItem.h"
#include "TLInteractionType.h"
#include "TLHealthPotion.generated.h"


UCLASS()
class UNREALPLAYGROUND_API ATLHealthPotion : public ATLGameItem
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATLHealthPotion();

// Custom Code
public:

	virtual void Interact_Implementation(APawn* InstigatorPawn, ETLInteractionType InteractionTypeUsed) override;
};
