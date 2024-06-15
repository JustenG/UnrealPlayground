// Fill out your copyright notice in the Description page of Project Settings.


#include "TLAttributeComponent.h"

// Sets default values for this component's properties
UTLAttributeComponent::UTLAttributeComponent()
{
	HealthMax = 100;
	Health = HealthMax;
}


bool UTLAttributeComponent::IsAlive() const
{
	return Health > 0.0f;
}


bool UTLAttributeComponent::IsFullHealth() const
{
	return Health >= HealthMax;
}


float UTLAttributeComponent::GetHealth() const
{
	return Health;
}


float UTLAttributeComponent::GetHealthMax() const
{
	return HealthMax;
}


float UTLAttributeComponent::GetHealthPercentage() const
{
	return Health / HealthMax;
}


bool UTLAttributeComponent::ApplyHealthChange(AActor* InstigatorActor, float Delta)
{
	float HealthOld = Health;
	float HealthNew = FMath::Clamp(Health + Delta, 0.0f, HealthMax);
	float DeltaClamped = HealthNew - HealthOld;
	
	Health = HealthNew;

	OnHealthChanged.Broadcast(InstigatorActor, this, Health, DeltaClamped);

	return DeltaClamped > 0;
}


UTLAttributeComponent* UTLAttributeComponent::GetAttributes(AActor* FromActor)
{
	if (FromActor)
	{
		return FromActor->FindComponentByClass<UTLAttributeComponent>();
	}

	return nullptr;
}


bool UTLAttributeComponent::IsActorAlive(AActor* Actor)
{
	UTLAttributeComponent* AttributeComp = GetAttributes(Actor);
	if (AttributeComp)
	{
		return AttributeComp->IsAlive();
	}

	return false;
}

