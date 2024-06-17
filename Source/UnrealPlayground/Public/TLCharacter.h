// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TLCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UTLInteractionComponent;
class UAnimMontage;
class UTLAttributeComponent;
class UTLActionComponent;


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

	virtual FVector GetPawnViewLocation() const override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void PostInitializeComponents() override;

// Custom Code
private: 

	FTimerHandle TimerHandle_SpawnAttack;

	TSubclassOf<AActor> LastRequestProjectileClass;

protected:
	
	UPROPERTY(VisibleAnywhere, Category = "Effects")
	FName TimeToHitParamName;

	UPROPERTY(VisibleAnywhere, Category = "Effects")
	FName AttackSpawnBoneParamName;

	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<AActor> ProjectileClassPrimary;

	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<AActor> ProjectileClassSecondary;

	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<AActor> ProjectileClassUltimate;

	UPROPERTY(EditAnywhere, Category  = "Attack")
	UAnimMontage* AttackAnim;

	UPROPERTY(EditDefaultsOnly, Category = "Attack")
	UParticleSystem* AttackVFX;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UTLInteractionComponent* InteractionComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UTLAttributeComponent* AttributeComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UTLActionComponent* ActionComp;

	void MoveForward(float Value);
	void MoveRight(float Value);

	void SprintStart();
	void SprintStop();

	void PrimaryAttack();
	void SecondaryAttack();
	void UltimateAttack();

	void PrimaryInteract();

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, UTLAttributeComponent* OwningComp, float NewHealth, float Delta);

	UFUNCTION()
	void OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

// Commands/Cheats
public:

	UFUNCTION(Exec)
	void HealSelf(float Amount = 0);

};
