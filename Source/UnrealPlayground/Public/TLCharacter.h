// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TLCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UTLInteractionComponent;
class UAnimMontage;

UCLASS()
class UNREALPLAYGROUND_API ATLCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATLCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

// Custom Code
protected:
	
	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<AActor> ProjectileClass;

	UPROPERTY(EditAnywhere, Category  = "Attack")
	UAnimMontage* AttackAnim;

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere)
	UTLInteractionComponent* InteractionComp;

	FTimerHandle TimerHandle_PrimaryAttack;

	void MoveForward(float Value);

	void MoveRight(float Value);

	void PrimaryAttack();
	void PrimaryAttack_TimeElapsed();	

	void PrimaryInteract();
};
