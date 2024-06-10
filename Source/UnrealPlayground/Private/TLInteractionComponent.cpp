// Fill out your copyright notice in the Description page of Project Settings.


#include "TLInteractionComponent.h"
#include "TLInteractionType.h"
#include "TLGameplayInterface.h"
#include "Camera/CameraComponent.h"
#include "Components/SphereComponent.h"
#include "DrawDebugHelpers.h"


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
	if (!MyOwner->GetWorldTimerManager().IsTimerActive(TimerHandle_PollRateDelay))
	{
		MyOwner->GetWorldTimerManager().SetTimer(TimerHandle_PollRateDelay, this, &UTLInteractionComponent::FindInteractiveObjects, PollRateDelay);
	}
}


void UTLInteractionComponent::FindInteractiveObjects()
{
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
			bool bSuccess = InteractContact(MyOwner, HitActor);
			if (!bSuccess)
			{
				ETLInteractionType TargetInteractionType = (ETLInteractionType)ITLGameplayInterface::Execute_GetInteractionType(HitActor);
				if (TargetInteractionType == ETLInteractionType::EBT_NEARBY)
				{
					InteractableActorsNearby.Add(HitActor);
				}
			}
		}
	}

	LastPollLocation = MyOwner->GetActorLocation();

	//DrawDebugSphere(GetWorld(), LastPollLocation, PollRadius, 32.0f, FColor::Emerald, false, 2.0f);
	//UE_LOG(LogTemp, Log, TEXT("Updated Players InteractableObjects: %d"), InteractableObjects.Num());
}


bool UTLInteractionComponent::InteractDirect() 
{
	AActor* MyOwner = GetOwner();

	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(CollisionChannel);

	FVector EyeLocation;
	FRotator EyeRotation;
	MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);
	FVector End = EyeLocation + (EyeRotation.Vector() * MaxInteractDistance);

	TArray<FHitResult> Hits;
	FCollisionShape Shape;
	Shape.SetSphere(MaxContactDistance);
	bool bBlockingHit = GetWorld()->SweepMultiByObjectType(Hits, EyeLocation, End, FQuat::Identity, ObjectQueryParams, Shape);

	FColor LineColor = bBlockingHit ? FColor::Green : FColor::Red;

	for (FHitResult Hit : Hits)
	{
		AActor* HitActor = Hit.GetActor();
		if (HitActor)
		{
			if (HitActor->Implements<UTLGameplayInterface>())
			{
				bool bSuccess = TryInteractWithType(MyOwner, HitActor, ETLInteractionType::EBT_DIRECT);
				if (bSuccess)
				{
					return true;
				}
			}
		}
	}
	return false;
}


bool UTLInteractionComponent::InteractNearby()
{
	if (InteractableActorsNearby.IsEmpty())
	{
		return false;
	}

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
	}

	if (ClosestActor)
	{
		APawn* MyPawn = Cast<APawn>(MyOwner);
		ITLGameplayInterface::Execute_Interact(ClosestActor, MyPawn);
		return true;
	}
	return false;
}


bool UTLInteractionComponent::InteractContact(AActor* Instigator, AActor* Target)
{
	bool bTouching = FVector::Distance(Instigator->GetActorLocation(), Target->GetActorLocation()) < MaxContactDistance;
	if (bTouching)
	{
		return TryInteractWithType(Instigator, Target, ETLInteractionType::EBT_CONTACT);
	}
	return false;
}


bool UTLInteractionComponent::TryInteractWithType(AActor* Instigator, AActor* Target, TEnumAsByte<ETLInteractionType> InteractionType)
{
	ETLInteractionType TargetInteractionType = ITLGameplayInterface::Execute_GetInteractionType(Target);
	if (TargetInteractionType == InteractionType)
	{
		ITLGameplayInterface::Execute_Interact(Target, Cast<APawn>(Instigator));
		return true;
	}
	return false;
}