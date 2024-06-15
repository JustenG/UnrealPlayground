// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/TLBTService_LowHealth.h"
#include "AIController.h"
#include "TLAttributeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

void UTLBTService_LowHealth::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AAIController* MyController = OwnerComp.GetAIOwner();
	if (ensure(MyController))
	{
		APawn* AIPawn = MyController->GetPawn();
		if (ensure(AIPawn))
		{
			UTLAttributeComponent* AttributeComp = UTLAttributeComponent::GetAttributes(AIPawn);
			if (ensure(AttributeComp))
			{
				const bool bLowHealth = AttributeComp->GetHealthPercentage() < HealthPercentageThreshold;
				UBlackboardComponent* BlackBoardComp = OwnerComp.GetBlackboardComponent();
				if (ensure(BlackBoardComp))
				{
					BlackBoardComp->SetValueAsBool(LowHealthKey.SelectedKeyName, bLowHealth);
				}
			}
		}
	}
}
