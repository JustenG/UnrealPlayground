// Fill out your copyright notice in the Description page of Project Settings.


#include "TLActionEffect.h"
#include "TLActionComponent.h"
#include "GameFramework/GameStateBase.h"


UTLActionEffect::UTLActionEffect()
{
	bAutoStart = true;
}


void UTLActionEffect::ExecutePeriodicEffect_Implementation(AActor* Instigator)
{

}


float UTLActionEffect::GetTimeRemaining() const
{
	AGameStateBase* GameState = GetWorld()->GetGameState<AGameStateBase>();
	if (GameState)
	{
		float EndTime = TimeStarted + Duration;
		return EndTime - GameState->GetServerWorldTimeSeconds();
	}
	return Duration;
}


void UTLActionEffect::StartAction(AActor* Instigator)
{
	Super::StartAction(Instigator);

	if (Duration > 0.0f)
	{
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "StopAction", Instigator);

		GetWorld()->GetTimerManager().SetTimer(DurationHandle, Delegate, Duration, false);
	}

	if (Period > 0.0f)
	{
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "ExecutePeriodicEffect", Instigator);

		GetWorld()->GetTimerManager().SetTimer(PeriodHandle, Delegate, Period, true);
	}
}


void UTLActionEffect::StopAction(AActor* Instigator)
{
	if (GetWorld()->GetTimerManager().GetTimerRemaining(PeriodHandle) < KINDA_SMALL_NUMBER)
	{
		ExecutePeriodicEffect(Instigator);
	}

	Super::StopAction(Instigator);

	GetWorld()->GetTimerManager().ClearTimer(PeriodHandle);
	GetWorld()->GetTimerManager().ClearTimer(DurationHandle);

	UTLActionComponent* Comp = GetOwningComponent();
	if (Comp)
	{
		Comp->RemoveAction(this);
	}
}
