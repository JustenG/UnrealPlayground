// Fill out your copyright notice in the Description page of Project Settings.


#include "TLAction.h"
#include "TLActionComponent.h"





UTLActionComponent* UTLAction::GetOwningComponent() const
{
	return Cast<UTLActionComponent>(GetOuter());
}


bool UTLAction::IsRunning() const
{
	return bIsRunning;
}


void UTLAction::OnStartAction_Implementation(AActor* Instigator)
{
	// Implement in derived class...
}


void UTLAction::OnStopAction_Implementation(AActor* Instigator)
{
	// Implement in derived class...
}


void UTLAction::BeforeStartAction_Implementation(AActor* Instigator)
{
	// Implement in derived class...
}


void UTLAction::BeforeStopAction_Implementation(AActor* Instigator)
{
	// Implement in derived class...
}


bool UTLAction::CanStart_Implementation(AActor* Instigator)
{
	if (IsRunning())
	{
		return false;
	}

	UTLActionComponent* Comp = GetOwningComponent();

	if (Comp->ActiveGameplayTags.HasAny(BlockedTags))
	{
		return false;
	}

	return true;
}


void UTLAction::StartAction(AActor* Instigator)
{
	BeforeStartAction(Instigator);

	UE_LOG(LogTemp, Log, TEXT("Running: %s"), *GetNameSafe(this));

	UTLActionComponent* Comp = GetOwningComponent();
	Comp->ActiveGameplayTags.AppendTags(GrantsTags);

	bIsRunning = true;

	OnStartAction(Instigator);
}


void UTLAction::StopAction(AActor* Instigator)
{
	BeforeStopAction(Instigator);

	UE_LOG(LogTemp, Log, TEXT("Stopped: %s"), *GetNameSafe(this));

	ensureAlways(bIsRunning);

	UTLActionComponent* Comp = GetOwningComponent();
	Comp->ActiveGameplayTags.RemoveTags(GrantsTags);

	bIsRunning = false;

	OnStopAction(Instigator);
}


UWorld* UTLAction::GetWorld() const
{
	// Outer is set when creating action via NewObject<T>
	UActorComponent* Comp = Cast<UActorComponent>(GetOuter());
	if (Comp)
	{
		return Comp->GetWorld();
	}

	return nullptr;
}
