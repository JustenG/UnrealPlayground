// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TLInteractionType.generated.h"


UENUM(BlueprintType, Category = "Interact")
enum ETLInteractionType : uint8 {
	EBT_INDIRECT = 0	UMETA(DisplayName = "Indirect"),
	EBT_DIRECT = 1		UMETA(DisplayName = "Direct"),
	EBT_NEARBY = 2		UMETA(DisplayName = "Nearby"),
	EBT_CONTACT = 3		UMETA(DisplayName = "Contact"),
};