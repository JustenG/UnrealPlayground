// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "TLInteractionType.h"
#include "TLGameplayInterface.generated.h"


// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UTLGameplayInterface : public UInterface
{
	GENERATED_BODY()
};


/**
 * 
 */
class UNREALPLAYGROUND_API ITLGameplayInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	/* Called after the Actor state was restored from a SaveGame file. */
	UFUNCTION(BlueprintNativeEvent)
	void OnActorLoaded();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	FText GetInteractText(APawn* InstigatorPawn);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Interact(APawn* InstigatorPawn, ETLInteractionType InteractionType);

};
