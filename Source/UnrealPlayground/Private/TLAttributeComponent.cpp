// Fill out your copyright notice in the Description page of Project Settings.


#include "TLAttributeComponent.h"
#include "TLGameModeBase.h"


static TAutoConsoleVariable<float> CVarDamageMultiplier(TEXT("tl.DamageMultiplier"), 1.0f, TEXT("Global Damage Modifier for Attribute Component."), ECVF_Cheat);



// Sets default values for this component's properties
UTLAttributeComponent::UTLAttributeComponent()
{
	HealthMax = 100;
	Health = HealthMax;
	RageMax = 20;
	Rage = 0;
}


bool UTLAttributeComponent::Kill(AActor* InstigatorActor)
{
	return ApplyHealthChange(InstigatorActor, -GetHealthMax());
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

float UTLAttributeComponent::GetRage() const
{
	return Rage;
}

float UTLAttributeComponent::GetRagehMax() const
{
	return RageMax;
}

bool UTLAttributeComponent::IsRageFull() const
{
	return Rage >= RageMax;
}


float UTLAttributeComponent::GetHealthPercentage() const
{
	return Health / HealthMax;
}


bool UTLAttributeComponent::ApplyHealthChange(AActor* InstigatorActor, float Delta)
{
	if (!GetOwner()->CanBeDamaged() && Delta < 0.0f)
	{
		return false;
	}

	if (Delta < 0.0f)
	{
		const float DamageMultiplier = CVarDamageMultiplier.GetValueOnGameThread();
		Delta *= DamageMultiplier;
	}

	float HealthOld = Health;
	float HealthNew = FMath::Clamp(Health + Delta, 0.0f, HealthMax);
	float DeltaClamped = HealthNew - HealthOld;
	
	Health = HealthNew;

	OnHealthChanged.Broadcast(InstigatorActor, this, Health, DeltaClamped);

	if (DeltaClamped < 0.0f && Health <= 0.0f)
	{
		ATLGameModeBase* GameMode = GetWorld()->GetAuthGameMode<ATLGameModeBase>();
		if (GameMode)
		{
			GameMode->OnActorKilled(GetOwner(), InstigatorActor);
		}
	}

	return DeltaClamped != 0;
}


bool UTLAttributeComponent::ApplyRageChange(AActor* InstigatorActor, float Delta)
{
	float RageOld = Rage;
	float RageNew = FMath::Clamp(Rage + Delta, 0.0f, RageMax);
	float DeltaClamped = RageNew - RageOld;
	Rage = RageNew;

	OnRageChanged.Broadcast(InstigatorActor, this, Rage, DeltaClamped);
	
	return DeltaClamped != 0;
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

