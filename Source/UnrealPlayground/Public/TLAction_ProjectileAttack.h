// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TLAction.h"
#include "TLAction_ProjectileAttack.generated.h"

class UAnimMontage;
class UParticleSystem;

/**
 * 
 */
UCLASS()
class UNREALPLAYGROUND_API UTLAction_ProjectileAttack : public UTLAction
{
	GENERATED_BODY()

public:

	UTLAction_ProjectileAttack();

// Custom Code
protected:

	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<AActor> ProjectileClass;

	UPROPERTY(VisibleAnywhere, Category = "Effects")
	FName HandSocketName;

	UPROPERTY(EditDefaultsOnly, Category = "Attack")
	float AttackAnimDelay;

	UPROPERTY(EditAnywhere, Category = "Attack")
	UAnimMontage* AttackAnim;

	/* Particle System played during attack animation */
	UPROPERTY(EditAnywhere, Category = "Attack")
	UParticleSystem* AttackVFX;

	UFUNCTION()
	void AttackDelay_Elapsed(ACharacter* InstigatorCharacter);

public:

	virtual void StartAction(AActor* Instigator) override;

};
