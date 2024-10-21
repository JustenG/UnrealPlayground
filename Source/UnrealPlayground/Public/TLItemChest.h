// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TLGameplayInterface.h"
#include "TLItemChest.generated.h"


class UStaticMeshComponent;

UCLASS()
class UNREALPLAYGROUND_API ATLItemChest : public AActor, public ITLGameplayInterface
{
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	ATLItemChest();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:

	UPROPERTY(EditAnywhere)
	float TargetPitch;

protected:

	UPROPERTY(ReplicatedUsing="OnRep_LidOpened", SaveGame)
	bool bLidOpened;

	UFUNCTION()
	void OnRep_LidOpened();

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> BaseMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> LidMesh;

public:

	virtual void OnActorLoaded_Implementation() override;

	virtual void Interact_Implementation(APawn* InstigatorPawn, ETLInteractionType InteractionTypeUsed) override;
};
