// Fill out your copyright notice in the Description page of Project Settings.


#include "TLActionEffect_Thorns.h"
#include "TLAttributeComponent.h"
#include "TLActionComponent.h"
#include "TLGameplayFunctionLibrary.h"
#include "GameFramework/Actor.h"

UTLActionEffect_Thorns::UTLActionEffect_Thorns()
{
	DamagePercentage = 0.10f;
}


void UTLActionEffect_Thorns::OnHealthChanged(AActor* InstigatorActor, UTLAttributeComponent* OwningComp, float NewHealth, float Delta)
{
	AActor* MyOwner = GetOwningComponent()->GetOwner();

	if (Delta > 0.0f && MyOwner == InstigatorActor)
	{
		return;
	}

	const int32 ThornDelta = FMath::RoundToInt( FMath::Abs(Delta) * DamagePercentage );
	if (ThornDelta > 0)
	{
		UTLGameplayFunctionLibrary::ApplyDamage(MyOwner, InstigatorActor, ThornDelta);
	}
}


void UTLActionEffect_Thorns::StartAction(AActor* Instigator)
{
	Super::StartAction(Instigator);

	UTLAttributeComponent* AttributeComp = Instigator->FindComponentByClass<UTLAttributeComponent>();
	if (AttributeComp)
	{
		AttributeComp->OnHealthChanged.AddDynamic(this, &UTLActionEffect_Thorns::OnHealthChanged);
	}
}


void UTLActionEffect_Thorns::StopAction(AActor* Instigator)
{
	Super::StopAction(Instigator);

	UTLAttributeComponent* AttributeComp = Instigator->FindComponentByClass<UTLAttributeComponent>();
	if (AttributeComp)
	{
		AttributeComp->OnHealthChanged.RemoveDynamic(this, &UTLActionEffect_Thorns::OnHealthChanged);
	}
}
