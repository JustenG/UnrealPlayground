// Fill out your copyright notice in the Description page of Project Settings.


#include "TLTargetDummy.h"
#include "TLAttributeComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(TLTargetDummy)

// Sets default values
ATLTargetDummy::ATLTargetDummy()
{
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	RootComponent = MeshComp;

	AttributeComp = CreateDefaultSubobject<UTLAttributeComponent>("AttributeComp");
}

void ATLTargetDummy::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// Trigger when health is changed (damage/healing)
	AttributeComp->OnHealthChanged.AddDynamic(this, &ATLTargetDummy::OnHealthChanged);
}


void ATLTargetDummy::OnHealthChanged(AActor* InstigatorActor, UTLAttributeComponent* OwningComp, float NewHealth, float Delta)
{
	if (Delta < 0.0f)
	{
		MeshComp->SetScalarParameterValueOnMaterials("TimeToHit", static_cast<float>(GetWorld()->TimeSeconds));
		UE_LOG(LogTemp, Log, TEXT("TimeSeconds: %f"), static_cast<float>(GetWorld()->TimeSeconds));
	}
}
