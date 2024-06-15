// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/TLAICharacter.h"
#include "Perception/PawnSensingComponent.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"
#include "TLAttributeComponent.h"
#include "BrainComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"


// Sets default values
ATLAICharacter::ATLAICharacter()
{
	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>("PawnSensingComp");
	AttributeComp = CreateDefaultSubobject<UTLAttributeComponent>("AttributeComp");

	TargetActorKey = "TargetActor";
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void ATLAICharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	PawnSensingComp->OnSeePawn.AddDynamic(this, &ATLAICharacter::OnPawnSeen);
	AttributeComp->OnHealthChanged.AddDynamic(this, &ATLAICharacter::OnHealthChanged);
}


// Called to bind functionality to input
void ATLAICharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


void ATLAICharacter::OnPawnSeen(APawn* Pawn)
{
	AAIController* AIC = GetController<AAIController>();
	if (AIC)
	{
		AIC->GetBlackboardComponent()->SetValueAsObject(TargetActorKey, Pawn);

		DrawDebugString(GetWorld(), GetActorLocation(), "PLAYER SPOTTED", nullptr, FColor::White, 0.5f, true);	
	}
}


void ATLAICharacter::OnHealthChanged(AActor* InstigatorActor, UTLAttributeComponent* OwningComp, float NewHealth, float Delta)
{
	if (Delta < 0.0f)
	{
		// Actor has Died
		if (NewHealth <= 0.0f)
		{
			AAIController* AIC = GetController<AAIController>();
			AIC->GetBrainComponent()->StopLogic("Killed");

			// Enable Actor Ragdoll
			GetMesh()->SetAllBodiesSimulatePhysics(true);
			GetMesh()->SetCollisionProfileName("Ragdoll");

			// Define when the Actor should be cleaned up by the engine
			SetLifeSpan(10.0f);
		}
	}
}

