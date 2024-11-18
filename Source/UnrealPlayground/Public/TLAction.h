// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameplayTagContainer.h"
#include "TLAction.generated.h"

class UWorld;


USTRUCT()
struct FActionRepData
{
	GENERATED_BODY()

public:

	UPROPERTY()
	bool bIsRunning;

	UPROPERTY()
	AActor* Instigator;
};


/**
 * 
 */
UCLASS(Blueprintable)
class UNREALPLAYGROUND_API UTLAction : public UObject
{
	GENERATED_BODY()

// Custom Code
protected:

	UPROPERTY(Replicated)
	float TimeStarted;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	UTexture2D* Icon;

	UPROPERTY(Replicated)
	UTLActionComponent* ActionComp;

	UPROPERTY(ReplicatedUsing = "OnRep_RepData")
	FActionRepData RepData;

	/* Tags added to owning actor when activated, removed when action stops */
	UPROPERTY(EditDefaultsOnly, Category = "Tags")
	FGameplayTagContainer GrantsTags;

	/* Action can only start if OwningActor has none of these Tags applied */
	UPROPERTY(EditDefaultsOnly, Category = "Tags")
	FGameplayTagContainer BlockedTags;

	UFUNCTION()
	void OnRep_RepData();

	UFUNCTION(BlueprintCallable, Category = "Action")
	UTLActionComponent* GetOwningComponent() const;

	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	void OnStartAction(AActor* Instigator);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Action")
	void OnStopAction(AActor* Instigator);

	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	void BeforeStartAction(AActor* Instigator);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Action")
	void BeforeStopAction(AActor* Instigator);

public:

	void Initialize(UTLActionComponent* NewActionComp);

	/* Action nickname to start/stop without a reference to the object */
	UPROPERTY(EditDefaultsOnly, Category = "Action")
	FName ActionName;

	/* Start immediately when added to an action component */
	UPROPERTY(EditDefaultsOnly, Category = "Action")
	bool bAutoStart;

	UFUNCTION(BlueprintCallable, Category = "Action")
	bool IsRunning() const;

	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	bool CanStart(AActor* Instigator);

	UFUNCTION()
	virtual void StartAction(AActor* Instigator);

	UFUNCTION()
	virtual void StopAction(AActor* Instigator);

	UWorld* GetWorld() const override;

	bool IsSupportedForNetworking() const override
	{
		return true;
	}
};
