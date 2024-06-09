// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TLTargetDummy.generated.h"

class UTLAttributeComponent;

UCLASS()
class UNREALPLAYGROUND_API ATLTargetDummy : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATLTargetDummy();

protected:

	virtual void PostInitializeComponents() override;

// Custom Code
protected:


	UPROPERTY(VisibleAnywhere)
	UTLAttributeComponent* AttributeComp;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* MeshComp;

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, UTLAttributeComponent* OwningComp, float NewHealth, float Delta);

};
