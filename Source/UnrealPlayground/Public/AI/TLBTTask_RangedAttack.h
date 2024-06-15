// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "TLBTTask_RangedAttack.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPLAYGROUND_API UTLBTTask_RangedAttack : public UBTTaskNode
{
	GENERATED_BODY()

public:

	UTLBTTask_RangedAttack();

private:

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:

	UPROPERTY(EditAnywhere, Category = "AI")
	FBlackboardKeySelector TargetActorKey;

	/* Max Random Bullet Spread (in Degrees) in positive and negative angle (shared between Yaw and Pitch) */
	UPROPERTY(EditAnywhere, Category = "AI")
	float MaxBulletSpread;

	UPROPERTY(EditAnywhere, Category = "AI")
	TSubclassOf<AActor> ProjectileClass;

	UPROPERTY(EditAnywhere, Category = "AI")
	FName MuzzleSocket;

};
