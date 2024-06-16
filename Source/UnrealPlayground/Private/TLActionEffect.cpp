// Fill out your copyright notice in the Description page of Project Settings.


#include "TLActionEffect.h"
#include "TLActionComponent.h"




UTLActionEffect::UTLActionEffect()
{
	bAutoStart = true;
}


void UTLActionEffect::ExecutePeriodicEffect_Implementation(AActor* Instigator)
{

}


void UTLActionEffect::OnStartAction_Implementation(AActor* Instigator)
{
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


void UTLActionEffect::OnStopAction_Implementation(AActor* Instigator)
{
	GetWorld()->GetTimerManager().ClearTimer(PeriodHandle);
	GetWorld()->GetTimerManager().ClearTimer(DurationHandle);

	UTLActionComponent* Comp = GetOwningComponent();
	if (Comp)
	{
		Comp->RemoveAction(this);
	}
}


void UTLActionEffect::BeforeStopAction_Implementation(AActor* Instigator)
{
	if (GetWorld()->GetTimerManager().GetTimerRemaining(PeriodHandle) < KINDA_SMALL_NUMBER)
	{
		ExecutePeriodicEffect(Instigator);
	}
}