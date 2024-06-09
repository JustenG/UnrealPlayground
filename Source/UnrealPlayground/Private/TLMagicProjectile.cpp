// Fill out your copyright notice in the Description page of Project Settings.


#include "TLMagicProjectile.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include <Kismet/GameplayStatics.h>

// Sets default values
ATLMagicProjectile::ATLMagicProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComp");
	SphereComp->SetCollisionProfileName("Projectile");
	RootComponent = SphereComp;

	EffectComp = CreateDefaultSubobject<UParticleSystemComponent>("EffectComp");
	EffectComp->SetupAttachment(SphereComp);

	MovementComp = CreateDefaultSubobject<UProjectileMovementComponent>("MovementComp");
	MovementComp->InitialSpeed = 5000.0f;
	MovementComp->bRotationFollowsVelocity = true;
	MovementComp->bInitialVelocityInLocalSpace = true;
}


// Called when the game starts or when spawned
void ATLMagicProjectile::BeginPlay()
{
	Super::BeginPlay();

}


void ATLMagicProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	AActor* MyInstigator = GetInstigator();
	if (MyInstigator)
	{
		SphereComp->IgnoreActorWhenMoving(MyInstigator, true);
	}

	// You should *never* bind your delegates in the constructor and choose either AActor::PostInitializeComponents() or BeginPlay() 
	// to avoid issues where delegates get serialized into the Blueprint and will still be called even when you later remove the delegate binding in C++.
	SphereComp->OnComponentHit.AddDynamic(this, &ATLMagicProjectile::OnActorHit);
}


// Called every frame
void ATLMagicProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void ATLMagicProjectile::OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	HandleImpact();
}


// _Implementation from it being marked as BlueprintNativeEvent
void ATLMagicProjectile::HandleImpact_Implementation()
{
	// Auto-managed particle pooling
	UGameplayStatics::SpawnEmitterAtLocation(this, ImpactVFX, GetActorLocation(), GetActorRotation(), true, EPSCPoolMethod::AutoRelease);
}