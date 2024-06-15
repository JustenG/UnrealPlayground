// Fill out your copyright notice in the Description page of Project Settings.


#include "TLMagicProjectile.h"

#include "Components/AudioComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Sound/SoundCue.h"
#include "TLAttributeComponent.h"

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

	AudioComp = CreateDefaultSubobject<UAudioComponent>("AudioComp");
	AudioComp->SetupAttachment(RootComponent);

	ImpactShakeRadius = 1500.0f;
	DamageAmount = 20.0f;

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
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &ATLMagicProjectile::OnActorOverlap);
}


// Called every frame
void ATLMagicProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void ATLMagicProjectile::OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor && OtherActor != GetInstigator())
		HandleMagicProjectileImpact();
}


void ATLMagicProjectile::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != GetInstigator())
	{
		UTLAttributeComponent* AttributeComp = Cast<UTLAttributeComponent>(OtherActor->GetComponentByClass(UTLAttributeComponent::StaticClass()));
		if (AttributeComp)
		{
			AttributeComp->ApplyHealthChange(GetInstigator(), - DamageAmount);
		}
		HandleMagicProjectileImpact();
	}
}


// _Implementation from it being marked as BlueprintNativeEvent
void ATLMagicProjectile::HandleMagicProjectileImpact_Implementation()
{
	// Auto-managed particle pooling
	UGameplayStatics::SpawnEmitterAtLocation(this, ImpactVFX, GetActorLocation(), GetActorRotation(), true, EPSCPoolMethod::AutoRelease);

	UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation());

	UGameplayStatics::PlayWorldCameraShake(this, ImpactShake, GetActorLocation(), 0, ImpactShakeRadius);

	Destroy();
}