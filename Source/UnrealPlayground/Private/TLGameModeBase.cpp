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
#include "TLSaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/GameStateBase.h"
#include "TLGameplayInterface.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"
#include "TLMonsterData.h"
#include "../UnrealPlayground.h"
#include "TLActionComponent.h"
#include "Engine/AssetManager.h"

static TAutoConsoleVariable<bool> CVarSpawnBots(TEXT("tl.SpawnBots"), true, TEXT("Enable spawning of bots via timer."), ECVF_Cheat);



ATLGameModeBase::ATLGameModeBase()
{
	PlayerStateClass = ATLPlayerState::StaticClass();
	SpawnTimerInterval = 2.0f;
	CreditsPerKill = 50;

	SlotName = "SaveGame01";
}


void ATLGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	LoadSaveGame();
}


void ATLGameModeBase::StartPlay()
{
	Super::StartPlay();

	GetWorldTimerManager().SetTimer(TimerHandle_SpawnBots, this, &ATLGameModeBase::SpawnBotTimerElapsed, SpawnTimerInterval, true);

	FEnvQueryRequest Request(SpawnPickupItemQuery, this); // See UEnvQueryManager::RunEQSQuery Line 1061
	Request.Execute(EEnvQueryRunMode::RandomBest5Pct, this, &ATLGameModeBase::SpawnPickupItemQueryCompleted); // See UEnvQueryInstanceBlueprintWrapper::RunQuery Line 130
	
	ApplyLoadedSaveGame();
}


void ATLGameModeBase::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	Super::HandleStartingNewPlayer_Implementation(NewPlayer);

	ATLPlayerState* PS = NewPlayer->GetPlayerState<ATLPlayerState>();
	if (PS)
	{
		PS->LoadPlayerState(CurrentSaveGame);
	}
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



void ATLGameModeBase::OnMonsterLoaded(FPrimaryAssetId LoadedId, FVector SpawnLocation)
{
	LogOnScreen(this, "Finished loading.", FColor::Green);

	UAssetManager* Manager = UAssetManager::GetIfValid();
	if (Manager)
	{
		UTLMonsterData* MonsterData = Cast<UTLMonsterData>(Manager->GetPrimaryAssetObject(LoadedId));
		if (MonsterData)
		{
			AActor* NewBot = GetWorld()->SpawnActor<AActor>(MonsterData->MonsterClass, SpawnLocation, FRotator::ZeroRotator);
			if (NewBot)
			{
				LogOnScreen(this, FString::Printf(TEXT("Spawned enemy: %s (%s)"), *GetNameSafe(NewBot), *GetNameSafe(MonsterData)));

				// Grant special actions, buffs etc.
				UTLActionComponent* ActionComp = Cast<UTLActionComponent>(NewBot->GetComponentByClass(UTLActionComponent::StaticClass()));
				if (ActionComp)
				{
					for (TSubclassOf<UTLAction> ActionClass : MonsterData->Actions)
					{
						ActionComp->AddAction(NewBot, ActionClass);
					}
				}
			}
		}
	}
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
		//GetWorld()->SpawnActor<AActor>(MinionClass, Locations[0], FRotator::ZeroRotator);

		if (MonsterTable)
		{
			TArray<FTLMonsterInfoRow*> Rows;
			MonsterTable->GetAllRows("", Rows);

			// Get Random Enemy
			int32 RandomIndex = FMath::RandRange(0, Rows.Num() - 1);
			FTLMonsterInfoRow* SelectedRow = Rows[RandomIndex];


			UAssetManager* Manager = UAssetManager::GetIfValid();
			if (Manager)
			{
				LogOnScreen(this, "Loading monster...", FColor::Green);

				TArray<FName> Bundles;
				FStreamableDelegate Delegate = FStreamableDelegate::CreateUObject(this, &ATLGameModeBase::OnMonsterLoaded, SelectedRow->MonsterId, Locations[0]);
				Manager->LoadPrimaryAsset(SelectedRow->MonsterId, Bundles, Delegate);
			}

		}
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


void ATLGameModeBase::WriteSaveGame()
{
	// Iterate all player states, we don't have proper ID to match yet (requires Steam or EOS)
	for (int32 i = 0; i < GameState->PlayerArray.Num(); i++)
	{
		ATLPlayerState* PS = Cast<ATLPlayerState>(GameState->PlayerArray[i]);
		if (PS)
		{
			PS->SavePlayerState(CurrentSaveGame);
			break; // single player only at this point
		}
	}

	CurrentSaveGame->SavedActors.Empty();

	// Iterate the entire world of actors
	for (FActorIterator It(GetWorld()); It; ++It)
	{
		AActor* Actor = *It;
		// Only interested in our 'gameplay actors'
		if (!Actor->Implements<UTLGameplayInterface>())
		{
			continue;
		}

		FActorSaveData ActorData;
		ActorData.ActorName = Actor->GetName();
		ActorData.Transform = Actor->GetActorTransform();

		FMemoryWriter MemWriter(ActorData.ByteData);
		FObjectAndNameAsStringProxyArchive Ar(MemWriter, true);
		Ar.ArIsSaveGame = true; // Find only variables with UPROPERTY(SaveGame)
		Actor->Serialize(Ar);// Converts Actor's SaveGame UPROPERTIES into binary array

		CurrentSaveGame->SavedActors.Add(ActorData);
	}

	UGameplayStatics::SaveGameToSlot(CurrentSaveGame, SlotName, 0);
}


void ATLGameModeBase::LoadSaveGame()
{
	if (UGameplayStatics::DoesSaveGameExist(SlotName, 0))
	{
		CurrentSaveGame = Cast<UTLSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, 0));
		if (CurrentSaveGame == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to load SaveGame Data."));
			return;
		}

		UE_LOG(LogTemp, Log, TEXT("Loaded SaveGame Data."));
	}
	else
	{
		CurrentSaveGame = Cast<UTLSaveGame>(UGameplayStatics::CreateSaveGameObject(UTLSaveGame::StaticClass()));

		UE_LOG(LogTemp, Log, TEXT("Created New SaveGame Data."));
	}
}


void ATLGameModeBase::ApplyLoadedSaveGame()
{
	// Iterate the entire world of actors
	for (FActorIterator It(GetWorld()); It; ++It)
	{
		AActor* Actor = *It;
		// Only interested in our 'gameplay actors'
		if (!Actor->Implements<UTLGameplayInterface>())
		{
			continue;
		}

		for (FActorSaveData ActorData : CurrentSaveGame->SavedActors)
		{
			if (ActorData.ActorName == Actor->GetName())
			{
				Actor->SetActorTransform(ActorData.Transform);

				FMemoryReader MemReader(ActorData.ByteData);
				FObjectAndNameAsStringProxyArchive Ar(MemReader, true);
				Ar.ArIsSaveGame = true;
				Actor->Serialize(Ar); // Convert binary array back into actor's variables

				ITLGameplayInterface::Execute_OnActorLoaded(Actor);

				break;
			}
		}
	}
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
