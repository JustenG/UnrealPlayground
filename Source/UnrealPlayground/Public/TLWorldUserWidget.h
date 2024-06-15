// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TLWorldUserWidget.generated.h"

class USizeBox;
class AActor;

/**
 * 
 */
UCLASS()
class UNREALPLAYGROUND_API UTLWorldUserWidget : public UUserWidget
{
	GENERATED_BODY()

protected:

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

protected:

	UPROPERTY(meta = (BindWidget))
	USizeBox* ParentSizeBox;

public:

	UPROPERTY(EditAnywhere, Category = "UI")
	FVector WorldOffset;

	UPROPERTY(BlueprintReadWrite, Category = "UI", meta = (ExposeOnSpawn = true))
	AActor* AttachedActor;
};
