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
#include "TLWorldUserWidget.h"


// Sets default values
ATLAICharacter::ATLAICharacter()
{
	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>("PawnSensingComp");
	AttributeComp = CreateDefaultSubobject<UTLAttributeComponent>("AttributeComp");

	TargetActorKey = "TargetActor";
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	TimeToHitParamName = "TimeToHit";

	GetMesh()->SetGenerateOverlapEvents(true);
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


void ATLAICharacter::SetTargetActor(AActor* NewTarget)
{
	AAIController* AIC = GetController<AAIController>();
	AIC->GetBlackboardComponent()->SetValueAsObject(TargetActorKey, NewTarget);
}


AActor* ATLAICharacter::GetTargetActor() const
{
	AAIController* AIC = GetController<AAIController>();
	return Cast<AActor>(AIC->GetBlackboardComponent()->GetValueAsObject(TargetActorKey));
}


void ATLAICharacter::OnPawnSeen(APawn* Pawn)
{
	AAIController* AIC = GetController<AAIController>();
	if (AIC)
	{
		AIC->GetBlackboardComponent()->SetValueAsObject(TargetActorKey, Pawn);

		if (!ActiveAlert && ensure(AlertWidgetClass))
		{
			ActiveAlert = CreateWidget<UTLWorldUserWidget>(GetWorld(), AlertWidgetClass);
			if (ActiveAlert)
			{
				ActiveAlert->AttachedActor = this;
				ActiveAlert->AddToViewport();
			}
		}
		DrawDebugString(GetWorld(), GetActorLocation(), "PLAYER SPOTTED", nullptr, FColor::White, 0.5f, true);	
	}
}


void ATLAICharacter::OnHealthChanged(AActor* InstigatorActor, UTLAttributeComponent* OwningComp, float NewHealth, float Delta)
{
	if (Delta < 0.0f)
	{
		if (InstigatorActor != this)
		{
			SetTargetActor(InstigatorActor);
		}

		if (ActiveHealthBar == nullptr && NewHealth > 0.0)
		{
			ActiveHealthBar = CreateWidget<UTLWorldUserWidget>(GetWorld(), HealthBarWidgetClass);
			if (ActiveHealthBar)
			{
				ActiveHealthBar->AttachedActor = this;
				ActiveHealthBar->AddToViewport();
			}
		}

		GetMesh()->SetScalarParameterValueOnMaterials(TimeToHitParamName, static_cast<float>(GetWorld()->TimeSeconds));

		// Actor has Died
		if (NewHealth <= 0.0f)
		{
			if (ActiveHealthBar != nullptr)
			{
				ActiveHealthBar->RemoveFromParent();
			}

			AAIController* AIC = GetController<AAIController>();
			AIC->GetBrainComponent()->StopLogic("Killed");

			// Enable Actor Ragdoll
			GetMesh()->SetAllBodiesSimulatePhysics(true);
			GetMesh()->SetCollisionProfileName("Ragdoll");

			GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			GetCharacterMovement()->DisableMovement();

			// Define when the Actor should be cleaned up by the engine
			SetLifeSpan(10.0f);
		}
	}
}

