// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TLAttributeComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnAttributeChanged, AActor*, InsitgatorActor, UTLAttributeComponent*, OwningComp, float, NewValue, float, Delta);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNREALPLAYGROUND_API UTLAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTLAttributeComponent();

// Custom Code
protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attributes" )
	float Health;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attributes")
	float HealthMax;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attributes")
	float Rage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attributes")
	float RageMax;

public:

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChanged OnHealthChanged;

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChanged OnRageChanged;

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	bool Kill(AActor* InstigatorActor);

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	bool IsAlive() const;

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	bool IsFullHealth() const;

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	float GetHealth() const;

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	float GetHealthMax() const;

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	float GetHealthPercentage() const;

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	float GetRage() const;

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	float GetRagehMax() const;

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	bool IsRageFull() const;

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	bool ApplyHealthChange(AActor* InstigatorActor, float Delta);

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	bool ApplyRageChange(AActor* InstigatorActor, float Delta);

// Statics
public:

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	static UTLAttributeComponent* GetAttributes(AActor* FromActor);

	UFUNCTION(BlueprintCallable, Category = "Attributes", meta = (DisplayName = "IsAlive"))
	static bool IsActorAlive(AActor* Actor);
};
