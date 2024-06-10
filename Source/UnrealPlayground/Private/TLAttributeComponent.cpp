// Fill out your copyright notice in the Description page of Project Settings.


#include "TLAttributeComponent.h"

// Sets default values for this component's properties
UTLAttributeComponent::UTLAttributeComponent()
{
	Health = 100;
}


bool UTLAttributeComponent::IsAlive() const
{
	return Health > 0.0f;
}

bool UTLAttributeComponent::ApplyHealthChange(float Delta)
{
	Health += Delta;

	OnHealthChanged.Broadcast(nullptr, this, Health, Delta);

	return true;
}

