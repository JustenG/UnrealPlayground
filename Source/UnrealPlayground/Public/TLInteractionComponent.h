// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include <TLInteractionType.h>
#include "TLInteractionComponent.generated.h"

class USphereComponent;
class UTLWorldUserWidget;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNREALPLAYGROUND_API UTLInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTLInteractionComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	//virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

// Custom Code
private:

	FTimerHandle TimerHandle_PollRateDelay;

	TArray<AActor*> InteractableActorsNearby;

	FVector LastPollLocation;

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Interact")
	float PollRateDelay;

	UPROPERTY(EditDefaultsOnly, Category = "Interact")
	float MaxInteractDistance;

	UPROPERTY(EditDefaultsOnly, Category = "Interact")
	float MaxContactDistance;

	UPROPERTY(EditDefaultsOnly, Category = "Interact")
	TEnumAsByte<ECollisionChannel> CollisionChannel;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UTLWorldUserWidget> InteractInfoWidgetClass;

	UPROPERTY()
	UTLWorldUserWidget* InteractInfoWidgetInstance;

public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	float GetMaxInteractDistance();

	void UpdateInteractUI(AActor* TargetActor);

	void FindInteractiveObjects();

	bool InteractDirect(FVector Start, FVector End);

	bool InteractDirect();

	bool InteractNearby();

	UFUNCTION(Server, Reliable)
	void ServerInteract(AActor* TargetActor, ETLInteractionType InteractionType);
};
