// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TLGameplayInterface.h"
#include "TLPickupItem.generated.h"

class UStaticMeshComponent;

UCLASS()
class UNREALPLAYGROUND_API ATLPickupItem : public AActor, public ITLGameplayInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATLPickupItem();

protected:

	//virtual void PostInitializeComponents() override;

// Custom Code
private:

	FTimerHandle TimerHandle_RespawnDelay;

protected:

	UPROPERTY(ReplicatedUsing="OnRep_IsActive")
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

	UFUNCTION()
	void OnRep_IsActive();

	void HideAndStartRespawnTimer();

	void RespawnGameItem();

public:

	bool GetIsActive();

	virtual void Interact_Implementation(APawn* InstigatorPawn, ETLInteractionType InteractionTypeUsed) override;

	virtual FText GetInteractText_Implementation(APawn* InstigatorPawn) override;
};
