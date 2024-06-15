// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/TLBTTask_HealActor.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "TLAttributeComponent.h"

EBTNodeResult::Type UTLBTTask_HealActor::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AActor* TargetActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TargetActorKey.SelectedKeyName));

	UTLAttributeComponent* AttributeComp = UTLAttributeComponent::GetAttributes(TargetActor);
	if (ensure(AttributeComp))
	{
		AttributeComp->ApplyHealthChange(AttributeComp->GetHealthMax());
	}

	return EBTNodeResult::Succeeded;
}
