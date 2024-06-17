// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TLAction.h"
#include "TLActionEffect.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPLAYGROUND_API UTLActionEffect : public UTLAction
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTLActionEffect();

// Custom Code
protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect")
	float Duration;

	/* Time between 'ticks' to apply effect */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect")
	float Period;

	FTimerHandle PeriodHandle;
	FTimerHandle DurationHandle;

	UFUNCTION(BlueprintNativeEvent, Category = "Effect")
	void ExecutePeriodicEffect(AActor* Instigator);

public:

	virtual void StartAction(AActor* Instigator) override;

	virtual void StopAction(AActor* Instigator) override;

};
