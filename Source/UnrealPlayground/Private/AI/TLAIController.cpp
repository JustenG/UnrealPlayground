// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/TLAIController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"


void ATLAIController::BeginPlay()
{
	Super::BeginPlay();

	if (ensureMsgf(BehaviorTree, TEXT("Behavior Tree is nullptr! Please assign BehaviorTree in your AI Controller.")))
	{
		RunBehaviorTree(BehaviorTree);
	}
}
