// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TLMagicProjectile.h"
#include "TLMagicProjectileTeleporter.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPLAYGROUND_API ATLMagicProjectileTeleporter : public ATLMagicProjectile
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATLMagicProjectileTeleporter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

// Custom Code
protected:

	UPROPERTY(EditDefaultsOnly, Category = "Teleport");
	float MaxTravelTime;

	UPROPERTY(EditDefaultsOnly, Category = "Teleport");
	float TeleportDelay;

	FTimerHandle TimerHandle_Teleport;

	virtual void HandleMagicProjectileImpact_Implementation() override;

	void TeleportInstigator();

};
