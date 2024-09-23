// Fill out your copyright notice in the Description page of Project Settings.


#include "TLInteractionComponent.h"
#include "TLInteractionType.h"
#include "TLGameplayInterface.h"
#include "Camera/CameraComponent.h"
#include "Components/SphereComponent.h"
#include "DrawDebugHelpers.h"


static TAutoConsoleVariable<bool> CVarDebugDrawInteraction(TEXT("tl.DebugDraw.Interaction"), false, TEXT("Enable Debug Lines for Interact Component."), ECVF_Cheat);



// Sets default values for this component's properties
UTLInteractionComponent::UTLInteractionComponent()
{
	PollRateDelay = 0.2f;
	MaxInteractDistance = 150.0f;
	MaxContactDistance = 50.0f;
	CollisionChannel = ECC_WorldDynamic;
}


void UTLInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	AActor* MyOwner = GetOwner();
	APawn* MyPawn = Cast<APawn>(MyOwner);
	if (MyPawn->IsLocallyControlled() && !MyOwner->GetWorldTimerManager().IsTimerActive(TimerHandle_PollRateDelay))
	{
		MyOwner->GetWorldTimerManager().SetTimer(TimerHandle_PollRateDelay, this, &UTLInteractionComponent::FindInteractiveObjects, PollRateDelay);
	}
}


float UTLInteractionComponent::GetMaxInteractDistance_Implementation()
{
	return MaxInteractDistance;
}

void UTLInteractionComponent::FindInteractiveObjects()
{
	const bool bDebugDraw = CVarDebugDrawInteraction.GetValueOnGameThread();

	AActor* MyOwner = GetOwner();

	MyOwner->GetWorldTimerManager().ClearTimer(TimerHandle_PollRateDelay);
	MyOwner->GetWorldTimerManager().SetTimer(TimerHandle_PollRateDelay, this, &UTLInteractionComponent::FindInteractiveObjects, PollRateDelay);

	FVector CurrentOwnerLocation = MyOwner->GetActorLocation();

	if (FVector::Distance(CurrentOwnerLocation, LastPollLocation) < MaxContactDistance)
	{
		return;
	}

	// Update InteractableObjects List
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(CollisionChannel);

	TArray<FHitResult> Hits;
	FCollisionShape Shape;
	Shape.SetSphere(MaxInteractDistance);
	GetWorld()->SweepMultiByObjectType(Hits, MyOwner->GetActorLocation(), MyOwner->GetActorLocation(), FQuat::Identity, ObjectQueryParams, Shape);

	InteractableActorsNearby.Empty();
	for (int i=0; i < Hits.Num(); ++i)
	{
		AActor* HitActor = Hits[i].GetActor();
		if (HitActor->Implements<UTLGameplayInterface>())
		{
			InteractableActorsNearby.Add(HitActor);
		}
	}

	LastPollLocation = MyOwner->GetActorLocation();

	if (bDebugDraw)
	{
		DrawDebugSphere(GetWorld(), LastPollLocation, MaxInteractDistance, 32.0f, FColor::Blue, false, 2.0f);
		UE_LOG(LogTemp, Log, TEXT("Updated Players InteractableObjects: %d"), InteractableActorsNearby.Num());
	}
}


bool UTLInteractionComponent::InteractDirect(FVector Start, FVector End)
{
	const bool bDebugDraw = CVarDebugDrawInteraction.GetValueOnGameThread();
	
	AActor* MyOwner = GetOwner();

	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(CollisionChannel);

	TArray<FHitResult> Hits;
	FCollisionShape Shape;
	Shape.SetSphere(MaxContactDistance);
	bool bBlockingHit = GetWorld()->SweepMultiByObjectType(Hits, Start, End, FQuat::Identity, ObjectQueryParams, Shape);

	FColor LineColor = bBlockingHit ? FColor::Green : FColor::Red;

	if (bDebugDraw)
	{
		DrawDebugLine(GetWorld(), Start, End, LineColor, false, 2.0f, 0, 0.0f);
	}

	FVector CurrentOwnerLocation = MyOwner->GetActorLocation();
	for (FHitResult Hit : Hits)
	{
		AActor* HitActor = Hit.GetActor();
		if (HitActor)
		{
			if (HitActor->Implements<UTLGameplayInterface>() && FVector::Distance(CurrentOwnerLocation, HitActor->GetActorLocation()) < MaxInteractDistance)
			{
				// Is Client?
				if (!GetOwner()->HasAuthority())
				{
					ITLGameplayInterface::Execute_Interact(HitActor, Cast<APawn>(MyOwner), ETLInteractionType::EBT_DIRECT);
				}
				ServerInteract(HitActor, ETLInteractionType::EBT_DIRECT);

				if (bDebugDraw)
				{
					DrawDebugSphere(GetWorld(), HitActor->GetActorLocation(), 12, 32, FColor::Green, false, 0.0f);
				}
				return true;
			}
		}
	}
	return false;
}


bool UTLInteractionComponent::InteractDirect() 
{
	AActor* MyOwner = GetOwner();

	FVector EyeLocation;
	FRotator EyeRotation;
	MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);
	FVector End = EyeLocation + (EyeRotation.Vector() * MaxInteractDistance);

	return InteractDirect(EyeLocation, End);
}


bool UTLInteractionComponent::InteractNearby()
{
	if (InteractableActorsNearby.IsEmpty())
	{
		return false;
	}

	const bool bDebugDraw = CVarDebugDrawInteraction.GetValueOnGameThread();

	AActor* MyOwner = GetOwner();
	FVector CurrentOwnerLocation = MyOwner->GetActorLocation();
	AActor* ClosestActor = nullptr;

	for (int i = 0; i < InteractableActorsNearby.Num(); ++i)
	{
		AActor* OtherActor = InteractableActorsNearby[i];
		if (ClosestActor == nullptr || FVector::Distance(CurrentOwnerLocation, OtherActor->GetActorLocation()) < FVector::Distance(CurrentOwnerLocation, ClosestActor->GetActorLocation()))
		{
			ClosestActor = OtherActor;
		}

		if (bDebugDraw)
		{
			DrawDebugSphere(GetWorld(), OtherActor->GetActorLocation(), 10, 16, FColor::Yellow, false, 0.0f);
		}
	}

	if (ClosestActor)
	{
		// Is Client?
		if (!GetOwner()->HasAuthority())
		{
			APawn* MyPawn = Cast<APawn>(MyOwner);
			ITLGameplayInterface::Execute_Interact(ClosestActor, MyPawn, ETLInteractionType::EBT_NEARBY);
		}
		ServerInteract(ClosestActor, ETLInteractionType::EBT_NEARBY);

		if (bDebugDraw)
		{
			DrawDebugSphere(GetWorld(), ClosestActor->GetActorLocation(), 12, 32, FColor::Green, false, 0.0f);
		}
		return true;
	}
	return false;
}


void UTLInteractionComponent::ServerInteract_Implementation(AActor* TargetActor, ETLInteractionType InteractionType)
{
	if (TargetActor == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, "No Focus Actor to interact.");
		return;
	}

	APawn* MyPawn = Cast<APawn>(GetOwner());
	ITLGameplayInterface::Execute_Interact(TargetActor, MyPawn, InteractionType);
}
