// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "TLGameModeBase.generated.h"


class UEnvQuery;
class UCurveFloat;

/**
 * 
 */
UCLASS()
class UNREALPLAYGROUND_API ATLGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:

	ATLGameModeBase();

	virtual void StartPlay() override;

// Custom Code
protected:

	FTimerHandle TimerHandle_SpawnBots;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	float CreditsPerKill;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	float SpawnTimerInterval;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UCurveFloat* DifficultyCurve;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	TSubclassOf<AActor> MinionClass;

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

	void SpawnBotQueryCompleted(TSharedPtr<FEnvQueryResult> Result); // See EnvQueryType.h Line 586 for FQueryFinishedSignature def
	
	void SpawnPickupItemQueryCompleted(TSharedPtr<FEnvQueryResult> Result); // See EnvQueryType.h Line 586 for FQueryFinishedSignature def

	UFUNCTION()
	void RespawnPlayerElapsed(AController* Controller);

public:

	virtual void OnActorKilled(AActor* VictimActor, AActor* Killer);

// Commands/Cheats
public:

	UFUNCTION(Exec)
	void KillAll();

};
