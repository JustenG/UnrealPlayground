// Fill out your copyright notice in the Description page of Project Settings.


#include "TLAction_ProjectileAttack.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"




UTLAction_ProjectileAttack::UTLAction_ProjectileAttack()
{
	HandSocketName = "Muzzle_01";
	AttackAnimDelay = 0.2f;
}


void UTLAction_ProjectileAttack::OnStartAction_Implementation(AActor* Instigator)
{
	ACharacter* Character = Cast<ACharacter>(Instigator);
	if (Character)
	{
		Character->PlayAnimMontage(AttackAnim);

		UGameplayStatics::SpawnEmitterAttached(AttackVFX, Character->GetMesh(), HandSocketName, FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::SnapToTarget);

		FTimerHandle TimerHandle_AttackDelay;
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "AttackDelay_Elapsed", Character);

		GetWorld()->GetTimerManager().SetTimer(TimerHandle_AttackDelay, Delegate, AttackAnimDelay, false);
	}
}


void UTLAction_ProjectileAttack::AttackDelay_Elapsed(ACharacter* InstigatorCharacter)
{
	if (ensureAlways(ProjectileClass))
	{
		FVector HandLocation = InstigatorCharacter->GetMesh()->GetSocketLocation(HandSocketName);

        FVector Start = InstigatorCharacter->GetPawnViewLocation();
        FVector Forward = InstigatorCharacter->GetControlRotation().Vector();
        FVector End = Start + (Forward * 5000);

        FHitResult Hit;
        FCollisionObjectQueryParams ObjectQueryParams;
        ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldStatic);
        ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
        ObjectQueryParams.AddObjectTypesToQuery(ECC_Pawn);
        GetWorld()->LineTraceSingleByObjectType(Hit, Start, End, ObjectQueryParams);

        FVector Traget = Hit.IsValidBlockingHit() ? Hit.ImpactPoint : End;
        FRotator ProjectileRotation = FRotationMatrix::MakeFromX(Traget - HandLocation).Rotator();

        FTransform PrimaryAttackSpawnTM = FTransform(ProjectileRotation, HandLocation);

        FActorSpawnParameters SpawnParams;
        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
        SpawnParams.Instigator = InstigatorCharacter;

        GetWorld()->SpawnActor<AActor>(ProjectileClass, PrimaryAttackSpawnTM, SpawnParams);
	}

	StopAction(InstigatorCharacter);
}
