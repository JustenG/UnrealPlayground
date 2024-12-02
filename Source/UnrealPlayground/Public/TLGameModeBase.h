// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "Engine/DataTable.h"
#include "TLGameModeBase.generated.h"


class UEnvQuery;
class UCurveFloat;
class UTLSaveGame;
class UDataTable;
class UTLMonsterData;


/* DataTable Row for spawning monsters in game mode  */
USTRUCT(BlueprintType)
struct FTLMonsterInfoRow : public FTableRowBase
{
	GENERATED_BODY()

public:

	FTLMonsterInfoRow()
	{
		Weight = 1.0f;
		SpawnCost = 5.0f;
		KillReward = 20.0f;
	}

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FPrimaryAssetId MonsterId;

	//TSubclassOf<AActor> MonsterClass;

	/* Relative chance to pick this monster */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Weight;

	/* Points required by gamemode to spawn this unit. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float SpawnCost;

	/* Amount of credits awarded to killer of this unit.  */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float KillReward;

};

/**
 * 
 */
UCLASS()
class UNREALPLAYGROUND_API ATLGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:

	ATLGameModeBase();

	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

	virtual void StartPlay() override;

	void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;

// Custom Code
protected:

	FTimerHandle TimerHandle_SpawnBots;

	FString SlotName;

	UPROPERTY()
	UTLSaveGame* CurrentSaveGame;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	float CreditsPerKill;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	float SpawnTimerInterval;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UCurveFloat* DifficultyCurve;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UDataTable* MonsterTable;
	//UPROPERTY(EditDefaultsOnly, Category = "AI")
	//TSubclassOf<AActor> MinionClass;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UEnvQuery* SpawnBotQuery;

	UPROPERTY(EditDefaultsOnly, Category = "Pickup Items")
	UEnvQuery* SpawnPickupItemQuery;

	UPROPERTY(EditDefaultsOnly, Category = "Pickup Items")
	int32 PickupItemSpawnCount;

	UPROPERTY(EditDefaultsOnly, Category = "Pickup Items")
	float PickupItemSpawnDistanceMin;

	UPROPERTY(EditDefaultsOnly, Category = "Pickup Items")
	TArray<TSubclassOf<AActor>> PickupItemTypes;

	void SpawnBotTimerElapsed();

	void OnMonsterLoaded(FPrimaryAssetId LoadedId, FVector SpawnLocation);

	void SpawnBotQueryCompleted(TSharedPtr<FEnvQueryResult> Result); // See EnvQueryType.h Line 586 for FQueryFinishedSignature def
	
	void SpawnPickupItemQueryCompleted(TSharedPtr<FEnvQueryResult> Result); // See EnvQueryType.h Line 586 for FQueryFinishedSignature def

	UFUNCTION()
	void RespawnPlayerElapsed(AController* Controller);

public:

	virtual void OnActorKilled(AActor* VictimActor, AActor* Killer);

	UFUNCTION(BlueprintCallable, Category = "SaveGame")
	void WriteSaveGame();

	void LoadSaveGame();

	void ApplyLoadedSaveGame();

// Commands/Cheats
public:

	UFUNCTION(Exec)
	void KillAll();

};
