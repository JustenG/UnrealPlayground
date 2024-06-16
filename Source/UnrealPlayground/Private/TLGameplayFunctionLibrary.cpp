// Fill out your copyright notice in the Description page of Project Settings.


#include "TLGameplayFunctionLibrary.h"
#include "TLAttributeComponent.h"


static TAutoConsoleVariable<bool> CVarDebugLogDamageMeshForce(TEXT("tl.LogDamageMeshForce"), false, TEXT("Turns on Debug Log messages for Directional Damage Mesh force."), ECVF_Cheat);


bool UTLGameplayFunctionLibrary::ApplyDamage(AActor* DamageCauser, AActor* TargetActor, float DamageAmount)
{
	UTLAttributeComponent* AttributeComp = UTLAttributeComponent::GetAttributes(TargetActor);
	if (AttributeComp)
	{
		return AttributeComp->ApplyHealthChange(DamageCauser, -DamageAmount);
	}
	return false;
}


bool UTLGameplayFunctionLibrary::ApplyDirectionalDamage(AActor* DamageCauser, AActor* TargetActor, float DamageAmount, const FHitResult& HitResult)
{
	if (ApplyDamage(DamageCauser, TargetActor, DamageAmount))
	{
		UPrimitiveComponent* HitComp = HitResult.GetComponent();
		if (HitComp && HitComp->IsSimulatingPhysics())
		{
			// Direction = Target - Origin
			FVector Direction = HitResult.TraceEnd - HitResult.TraceStart;
			Direction.Normalize();

			HitComp->AddImpulseAtLocation(Direction * 300000.f, HitResult.ImpactPoint);
			if (CVarDebugLogDamageMeshForce.GetValueOnGameThread())
			{
				UE_LOG(LogTemp, Warning, TEXT("Force Applied to %s."), *HitComp->GetName());
			}
		}
		else
		{
			if (CVarDebugLogDamageMeshForce.GetValueOnGameThread())
			{
				UE_LOG(LogTemp, Warning, TEXT("Force NOT allowed to %s."), *HitComp->GetName());
			}
		}
		return true;
	}

	return false;
}