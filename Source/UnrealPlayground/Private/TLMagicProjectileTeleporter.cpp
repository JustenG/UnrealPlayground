// Fill out your copyright notice in the Description page of Project Settings.


#include "TLMagicProjectileTeleporter.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include <Kismet/GameplayStatics.h>


ATLMagicProjectileTeleporter::ATLMagicProjectileTeleporter() : ATLMagicProjectile()
{
	MaxTravelTime = 0.2f;
	TeleportDelay = 0.2f;
}


void ATLMagicProjectileTeleporter::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(TimerHandle_Teleport, this, &ATLMagicProjectileTeleporter::HandleMagicProjectileImpact, MaxTravelTime);
}


void ATLMagicProjectileTeleporter::HandleMagicProjectileImpact_Implementation()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_Teleport);
	GetWorldTimerManager().SetTimer(TimerHandle_Teleport, this, &ATLMagicProjectileTeleporter::TeleportInstigator, TeleportDelay);
	
	MovementComp->StopMovementImmediately();
	SetActorEnableCollision(false);

	EffectComp->DeactivateSystem();

	// Auto-managed particle pooling
	UGameplayStatics::SpawnEmitterAtLocation(this, ImpactVFX, GetActorLocation(), GetActorRotation(), true, EPSCPoolMethod::AutoRelease);
}


void ATLMagicProjectileTeleporter::TeleportInstigator()
{
	AActor* MyInstigator = GetInstigator();
	check(MyInstigator); // Assert

	// Rather than setting the location of the actor, we can use TeleportTo, which does extra safety checks, here we explicilty turn off checks
	MyInstigator->TeleportTo(GetActorLocation(), MyInstigator->GetActorRotation(), false, true);

	Destroy();
}
