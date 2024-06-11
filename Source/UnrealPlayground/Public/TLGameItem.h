// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TLGameplayInterface.h"
#include "TLGameItem.generated.h"

class UStaticMeshComponent;

UCLASS()
class UNREALPLAYGROUND_API ATLGameItem : public AActor, public ITLGameplayInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATLGameItem();

protected:

	//virtual void PostInitializeComponents() override;

// Custom Code
private:

	FTimerHandle TimerHandle_RespawnDelay;

protected:

	bool bIsActive;

	UPROPERTY(EditDefaultsOnly, Category = "Game Item")
	bool bRespawns;

	UPROPERTY(EditDefaultsOnly, meta = (EditCondition = "bRespawns"), Category = "Game Item")
	float RespawnDelay;

	UPROPERTY(EditDefaultsOnly, Category = "Game Item")
	TEnumAsByte<ETLInteractionType> InteractionType;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> MeshComp;

	//UFUNCTION()
	//void OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void UseGameItem();

	void RespawnGameItem();

public:

	virtual void Interact_Implementation(APawn* InstigatorPawn, ETLInteractionType InteractionTypeUsed) override;
};
