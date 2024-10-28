// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TLPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPLAYGROUND_API ATLPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:

	virtual void BeginPlayingState() override;

	void SetupInputComponent() override;

// Custom Code
protected:

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> PauseMenuClass;

	UPROPERTY()
	UUserWidget* PauseMenuInstance;


	UFUNCTION(BlueprintImplementableEvent)
	void BlueprintBeginPlayingState();

	UFUNCTION(BlueprintCallable)
	void TogglePauseMenu();

	
};
