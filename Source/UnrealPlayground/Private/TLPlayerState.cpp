// Fill out your copyright notice in the Description page of Project Settings.


#include "TLPlayerState.h"
#include "../UnrealPlayground.h"
#include "Net/UnrealNetwork.h"
#include "TLSaveGame.h"

void ATLPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATLPlayerState, Credits);
}


void ATLPlayerState::OnRep_Credits(int32 OldCredits)
{
	OnCreditsChanged.Broadcast(this, Credits, Credits - OldCredits);

	FString ActionMsg = FString::Printf(TEXT("[%s] Credits: %d"), *GetNameSafe(GetOwner()), Credits);
	LogOnScreen(this, ActionMsg, FColor::Black, 10.0f);
}


void ATLPlayerState::AddCredits(int32 Delta)
{
	if (!ensure(Delta >= 0.0f))
	{
		return;
	}

	Credits += Delta;
	OnCreditsChanged.Broadcast(this, Credits, Delta);

	FString ActionMsg = FString::Printf(TEXT("[%s] Credits: %d"), *GetNameSafe(GetOwner()), Credits);
	LogOnScreen(this, ActionMsg, FColor::Black, 10.0f);
}


bool ATLPlayerState::RemoveCredits(int32 Delta)
{
	if (!ensure(Delta >= 0.0f))
	{
		return false;
	}

	if (Credits < Delta)
	{
		return false;
	}

	Credits -= Delta;
	OnCreditsChanged.Broadcast(this, Credits, -Delta);
	return true;
}


int32 ATLPlayerState::GetCredits() const
{
	return Credits;
}

void ATLPlayerState::SavePlayerState_Implementation(UTLSaveGame* SaveObject)
{
	if (SaveObject)
	{
		SaveObject->Credits = Credits;
	}
}


void ATLPlayerState::LoadPlayerState_Implementation(UTLSaveGame* SaveObject)
{
	if (SaveObject)
	{
		Credits = SaveObject->Credits;
		OnCreditsChanged.Broadcast(this, Credits, 0);
	}
}
