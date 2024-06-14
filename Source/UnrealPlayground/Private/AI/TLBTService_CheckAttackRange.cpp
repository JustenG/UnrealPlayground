// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/TLBTService_CheckAttackRange.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"


void UTLBTService_CheckAttackRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	UBlackboardComponent* BlackBoardComp = OwnerComp.GetBlackboardComponent();
	check(BlackBoardComp);

	AActor* TargetActor = Cast<AActor>(BlackBoardComp->GetValueAsObject(TargetActorKey.SelectedKeyName));
	if (TargetActor)
	{
		AAIController* MyController = OwnerComp.GetAIOwner();
		check(MyController);

		APawn* AIPawn = MyController->GetPawn();
		if (ensure(AIPawn))
		{
			const float DistanceTo = FVector::Distance(TargetActor->GetActorLocation(), MyController->GetPawn()->GetActorLocation());
			const bool bWithinRange = DistanceTo < MaxAttackRange;

			bool bHasLOS = false;
			if (bWithinRange)
			{
				bHasLOS = MyController->LineOfSightTo(TargetActor);
			}

			BlackBoardComp->SetValueAsBool(AttackRangeKey.SelectedKeyName, bWithinRange && bHasLOS);
		}
	}
}
