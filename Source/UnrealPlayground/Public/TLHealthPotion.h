// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TLGameplayInterface.h"
#include "TLInteractionType.h"
#include "TLHealthPotion.generated.h"

class UStaticMeshComponent;
class USphereComponent;

UCLASS()
class UNREALPLAYGROUND_API ATLHealthPotion : public AActor, public ITLGameplayInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATLHealthPotion();

protected:
	// Called when the game starts or when spawned
	//virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;

public:	
	// Called every frame
	//virtual void Tick(float DeltaTime) override;

// Custom Code
private:

	FTimerHandle TimerHandle_RespawnDelay;

	ETLInteractionType GetInteractionType_Implementation();

	void Interact_Implementation(APawn* InstigatorPawn);

	void TogglePotion();

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Health Potion")
	float RespawnDelay;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component");
	TObjectPtr<UStaticMeshComponent> MeshComp;

	UFUNCTION()
	void OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
