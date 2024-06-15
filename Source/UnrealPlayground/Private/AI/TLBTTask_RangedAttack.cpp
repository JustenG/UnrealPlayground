// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/TLBTTask_RangedAttack.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "GameFramework/Character.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "TLAttributeComponent.h"


UTLBTTask_RangedAttack::UTLBTTask_RangedAttack()
{
	MuzzleSocket = "Muzzle_01";
	MaxBulletSpread = 2.0f;

	TargetActorKey.SelectedKeyName = "TargetActor";
}


EBTNodeResult::Type UTLBTTask_RangedAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ACharacter* MyCharacter = CastChecked<ACharacter>(OwnerComp.GetAIOwner()->GetPawn());
	AActor* TargetActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TargetActorKey.SelectedKeyName));

	if (TargetActor == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	if (!UTLAttributeComponent::IsActorAlive(TargetActor))
	{
		return EBTNodeResult::Failed;
	}

	const FVector MuzzleLocation = MyCharacter->GetMesh()->GetSocketLocation(MuzzleSocket);
	const FVector Direction = TargetActor->GetActorLocation() - MuzzleLocation;
	FRotator MuzzleRotation = Direction.Rotation();

	// Make Attack less accurate
	MuzzleRotation.Pitch += FMath::RandRange(0.0f, MaxBulletSpread);
	MuzzleRotation.Yaw += FMath::RandRange(-MaxBulletSpread, MaxBulletSpread);

	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	Params.Instigator = MyCharacter;

	AActor* NewProj = GetWorld()->SpawnActor<AActor>(ProjectileClass, MuzzleLocation, MuzzleRotation, Params);
	return NewProj ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;
}