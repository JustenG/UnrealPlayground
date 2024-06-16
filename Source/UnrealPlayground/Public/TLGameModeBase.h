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
	float SpawnTimerInterval;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UCurveFloat* DifficultyCurve;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	TSubclassOf<AActor> MinionClass;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UEnvQuery* SpawnBotQuery;

	void SpawnBotTimerElapsed();

	void OnQueryCompleted(TSharedPtr<FEnvQueryResult> Result); // See EnvQueryType.h Line 586 for FQueryFinishedSignature def

// Commands/Cheats
public:

	UFUNCTION(Exec)
	void KillAll();

};
