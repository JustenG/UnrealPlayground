// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TLGameItem.generated.h"

class ETLInteractionType;
class UStaticMeshComponent;

UCLASS()
class UNREALPLAYGROUND_API ATLGameItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATLGameItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

// Custom Code
private:

	FTimerHandle TimerHandle_RespawnDelay;

	void Interact_Implementation(APawn* InstigatorPawn);

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Game Item")
	bool bRespawns;

	UPROPERTY(EditDefaultsOnly, meta = (EditCondition = "bRespawns"), Category = "Game Item")
	float RespawnDelay;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> MeshComp;

};
