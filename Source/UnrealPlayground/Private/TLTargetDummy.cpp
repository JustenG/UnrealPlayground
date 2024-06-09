// Fill out your copyright notice in the Description page of Project Settings.


#include "TLTargetDummy.h"
#include "TLAttributeComponent.h"

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
		MeshComp->SetScalarParameterValueOnMaterials("TimeToHit", GetWorld()->TimeSeconds);
	}
}
