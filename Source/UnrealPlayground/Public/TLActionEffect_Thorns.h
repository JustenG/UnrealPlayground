// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TLActionEffect.h"
#include "TLActionEffect_Thorns.generated.h"

class UTLAttributeComponent;
/**
 * 
 */
UCLASS()
class UNREALPLAYGROUND_API UTLActionEffect_Thorns : public UTLActionEffect
{
	GENERATED_BODY()

public:

	UTLActionEffect_Thorns();

// Custom Code
protected:

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float DamagePercentage;

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, UTLAttributeComponent* OwningComp, float NewHealth, float Delta);

public:

	virtual void StartAction(AActor* Instigator) override;

	virtual void StopAction(AActor* Instigator) override;
	
};
