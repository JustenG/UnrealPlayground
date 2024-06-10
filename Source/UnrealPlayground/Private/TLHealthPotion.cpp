// Fill out your copyright notice in the Description page of Project Settings.


#include "TLHealthPotion.h"

#include "Components/SphereComponent.h"
#include "TLAttributeComponent.h"
#include "TLInteractionComponent.h"
#include "TLInteractionType.h"
#include "DrawDebugHelpers.h"


// Sets default values
ATLHealthPotion::ATLHealthPotion()
{
    RespawnDelay = 10.0f;

    MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
    RootComponent = MeshComp;
}

void ATLHealthPotion::PostInitializeComponents()
{
    MeshComp->OnComponentBeginOverlap.AddDynamic(this, &ATLHealthPotion::OnActorOverlap);
}


ETLInteractionType ATLHealthPotion::GetInteractionType_Implementation()
{
    return ETLInteractionType::EBT_CONTACT;
}


void ATLHealthPotion::Interact_Implementation(APawn* InstigatorPawn)
{
    if (GetWorldTimerManager().IsTimerActive(TimerHandle_RespawnDelay))
    {
        return;
    }

    UTLAttributeComponent* AttributeComp = InstigatorPawn->GetComponentByClass<UTLAttributeComponent>();

    if (!AttributeComp->IsFullHealth())
    {
        AttributeComp->ApplyHealthChange(AttributeComp->GetHealthMax());
        UE_LOG(LogTemp, Log, TEXT("Player Healed"));

        GetWorldTimerManager().ClearTimer(TimerHandle_RespawnDelay);
        GetWorldTimerManager().SetTimer(TimerHandle_RespawnDelay, this, &ATLHealthPotion::TogglePotion, RespawnDelay);
        TogglePotion();
    }
}


void ATLHealthPotion::TogglePotion()
{
    RootComponent->SetVisibility(!RootComponent->IsVisible(), true);
}


void ATLHealthPotion::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    UTLInteractionComponent* InteractionComp = Cast<UTLInteractionComponent>(OtherActor->GetComponentByClass(UTLInteractionComponent::StaticClass()));
    if (InteractionComp)
    {
        InteractionComp->TryInteractWithType(OtherActor, this, ETLInteractionType::EBT_CONTACT);
    }
}

