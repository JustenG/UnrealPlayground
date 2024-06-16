// Fill out your copyright notice in the Description page of Project Settings.


#include "TLGameModeBase.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "DrawDebugHelpers.h"
#include "AI/TLAICharacter.h"
#include "TLAttributeComponent.h"
#include "EngineUtils.h"
#include "TLCharacter.h"
#include "TLPlayerState.h"


static TAutoConsoleVariable<bool> CVarSpawnBots(TEXT("tl.SpawnBots"), true, TEXT("Enable spawning of bots via timer."), ECVF_Cheat);



ATLGameModeBase::ATLGameModeBase()
{
	PlayerStateClass = ATLPlayerState::StaticClass();
	SpawnTimerInterval = 2.0f;
	CreditsPerKill = 50;
}


void ATLGameModeBase::StartPlay()
{
	Super::StartPlay();

	GetWorldTimerManager().SetTimer(TimerHandle_SpawnBots, this, &ATLGameModeBase::SpawnBotTimerElapsed, SpawnTimerInterval, true);

	FEnvQueryRequest Request(SpawnPickupItemQuery, this); // See UEnvQueryManager::RunEQSQuery Line 1061
	Request.Execute(EEnvQueryRunMode::RandomBest5Pct, this, &ATLGameModeBase::SpawnPickupItemQueryCompleted); // See UEnvQueryInstanceBlueprintWrapper::RunQuery Line 130
}


void ATLGameModeBase::SpawnBotTimerElapsed()
{
	if (!CVarSpawnBots.GetValueOnGameThread())
	{
		return;
	}

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
	Request.Execute(EEnvQueryRunMode::RandomBest5Pct, this, &ATLGameModeBase::SpawnBotQueryCompleted); // See UEnvQueryInstanceBlueprintWrapper::RunQuery Line 130
}


void ATLGameModeBase::SpawnBotQueryCompleted(TSharedPtr<FEnvQueryResult> Result)
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


void ATLGameModeBase::SpawnPickupItemQueryCompleted(TSharedPtr<FEnvQueryResult> Result)
{
	FEnvQueryResult* QueryResult = Result.Get();
	if (!QueryResult->IsSuccessful())
	{
		UE_LOG(LogTemp, Warning, TEXT("Spawn Pickup Items EQS Query Failed!"));
		return;
	}

	TArray<FVector> Locations;
	QueryResult->GetAllAsLocations(Locations);

	TArray<FVector> SpawnLocations;
	while (SpawnLocations.Num() < PickupItemSpawnCount)
	{
		int32 Index = FMath::RandRange(0, Locations.Num() - 1);
		FVector TargetLocation = Locations[Index];
		Locations.RemoveAt(Index);

		bool bValidLocation = true;
		for (FVector OtherLocation : SpawnLocations)
		{
			float Distance = (TargetLocation - OtherLocation).Size();
			if (Distance < PickupItemSpawnDistanceMin)
			{
				bValidLocation = false;
				break;
			}
		}

		if (bValidLocation)
		{
			SpawnLocations.Add(TargetLocation);
		}
	}

	for (int i = 0; i < SpawnLocations.Num(); ++i)
	{
		int32 PickupItemIndex = FMath::RandRange(0, PickupItemTypes.Num() - 1);
		TSubclassOf<AActor> PickupItemType = PickupItemTypes[PickupItemIndex];
		GetWorld()->SpawnActor<AActor>(PickupItemType, SpawnLocations[i], FRotator::ZeroRotator);
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

	APawn* KillerPawn = Cast<APawn>(Killer);
	if (KillerPawn && KillerPawn != VictimActor)
	{
		ATLPlayerState* PlayerState = KillerPawn->GetPlayerState<ATLPlayerState>();
		if (PlayerState)
		{
			PlayerState->AddCredits(CreditsPerKill);
		}
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
