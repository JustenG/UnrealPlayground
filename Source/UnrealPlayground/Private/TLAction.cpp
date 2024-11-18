// Fill out your copyright notice in the Description page of Project Settings.


#include "TLAction.h"
#include "TLActionComponent.h"
#include "../UnrealPlayground.h"
#include "Net/UnrealNetwork.h"


void UTLAction::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UTLAction, RepData);
	DOREPLIFETIME(UTLAction, ActionComp);
	DOREPLIFETIME(UTLAction, TimeStarted);
}


void UTLAction::OnRep_RepData()
{
	if (RepData.bIsRunning)
	{
		StartAction(RepData.Instigator);
	}
	else
	{
		StopAction(RepData.Instigator);
	}
}


UTLActionComponent* UTLAction::GetOwningComponent() const
{
	//return Cast<UTLActionComponent>(GetOuter());

	return ActionComp;
}


void UTLAction::Initialize(UTLActionComponent* NewActionComp)
{
	ActionComp = NewActionComp;
}


bool UTLAction::IsRunning() const
{
	return RepData.bIsRunning;
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

	//UE_LOG(LogTemp, Log, TEXT("Running: %s"), *GetNameSafe(this));
	LogOnScreen(this, FString::Printf(TEXT("Started: %s"), *ActionName.ToString()), FColor::Green);

	UTLActionComponent* Comp = GetOwningComponent();
	Comp->ActiveGameplayTags.AppendTags(GrantsTags);

	RepData.bIsRunning = true;
	RepData.Instigator = Instigator;

	if (GetOwningComponent()->GetOwnerRole() == ROLE_Authority)
	{
		TimeStarted = static_cast<float>(GetWorld()->TimeSeconds);
	}

	GetOwningComponent()->OnActionStarted.Broadcast(GetOwningComponent(), this );

	OnStartAction(Instigator);
}


void UTLAction::StopAction(AActor* Instigator)
{
	BeforeStopAction(Instigator);

	//UE_LOG(LogTemp, Log, TEXT("Stopped: %s"), *GetNameSafe(this));
	LogOnScreen(this, FString::Printf(TEXT("Stopped: %s"), *ActionName.ToString()), FColor::White);

	//ensureAlways(bIsRunning);

	UTLActionComponent* Comp = GetOwningComponent();
	Comp->ActiveGameplayTags.RemoveTags(GrantsTags);

	RepData.bIsRunning = false;
	RepData.Instigator = Instigator;

	GetOwningComponent()->OnActionStopped.Broadcast(GetOwningComponent(), this);

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
