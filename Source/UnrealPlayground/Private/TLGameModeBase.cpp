// Fill out your copyright notice in the Description page of Project Settings.


#include "TLGameModeBase.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "DrawDebugHelpers.h"
#include "AI/TLAICharacter.h"
#include "TLAttributeComponent.h"
#include "EngineUtils.h"
#include "TLCharacter.h"


ATLGameModeBase::ATLGameModeBase()
{
	SpawnTimerInterval = 2.0f;
}


void ATLGameModeBase::StartPlay()
{
	Super::StartPlay();

	GetWorldTimerManager().SetTimer(TimerHandle_SpawnBots, this, &ATLGameModeBase::SpawnBotTimerElapsed, SpawnTimerInterval, true);
}


void ATLGameModeBase::SpawnBotTimerElapsed()
{
	int32 BotCountAlive = 0;
	for (TActorIterator<ATLAICharacter> Iterator(GetWorld()); Iterator; ++Iterator)
	{
		ATLAICharacter* Bot = *Iterator;
		UTLAttributeComponent* AttributeComp = UTLAttributeComponent::GetAttributes(Bot);
		if (ensure(AttributeComp) && AttributeComp->IsAlive())
		{
			BotCountAlive++;
		}
	}

	const float BotCountMax = DifficultyCurve->GetFloatValue(GetWorld()->TimeSeconds);
	if (BotCountAlive >= BotCountMax)
	{
		return;
	}

	FEnvQueryRequest Request(SpawnBotQuery, this); // See UEnvQueryManager::RunEQSQuery Line 1061
	Request.Execute(EEnvQueryRunMode::RandomBest5Pct, this, &ATLGameModeBase::OnQueryCompleted); // See UEnvQueryInstanceBlueprintWrapper::RunQuery Line 130
}


void ATLGameModeBase::OnQueryCompleted(TSharedPtr<FEnvQueryResult> Result)
{
	FEnvQueryResult* QueryResult = Result.Get();
	if (!QueryResult->IsSuccessful())
	{
		UE_LOG(LogTemp, Warning, TEXT("Spawn bot EQS Query Failed!"));
		return;
	}

	TArray<FVector> Locations;
	QueryResult->GetAllAsLocations(Locations);

	if (Locations.IsValidIndex(0))
	{
		GetWorld()->SpawnActor<AActor>(MinionClass, Locations[0], FRotator::ZeroRotator);
	}
}


void ATLGameModeBase::RespawnPlayerElapsed(AController* Controller)
{
	if (ensure(Controller))
	{
		Controller->UnPossess();

		RestartPlayer(Controller);
	}
}


void ATLGameModeBase::OnActorKilled(AActor* VictimActor, AActor* Killer)
{
	ATLCharacter* PlayerCharacter = Cast<ATLCharacter>(VictimActor);
	if (PlayerCharacter)
	{
		FTimerHandle TimerHandle_RespawnDelay;
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "RespawnPlayerElapsed", PlayerCharacter->GetController());

		float RespawnDelay = 2.0f;
		GetWorldTimerManager().SetTimer(TimerHandle_RespawnDelay, Delegate, RespawnDelay, false);

	}
	
	UE_LOG(LogTemp, Log, TEXT("OnActorKilled: Victim: %s, Killer: %s"), *GetNameSafe(VictimActor), *GetNameSafe(Killer));
}


void ATLGameModeBase::KillAll()
{
	for (TActorIterator<ATLAICharacter> Iterator(GetWorld()); Iterator; ++Iterator)
	{
		ATLAICharacter* Bot = *Iterator;
		UTLAttributeComponent* AttributeComp = UTLAttributeComponent::GetAttributes(Bot);
		if (ensure(AttributeComp) && AttributeComp->IsAlive())
		{
			AttributeComp->Kill(this);
		}
	}
}
