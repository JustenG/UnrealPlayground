// Fill out your copyright notice in the Description page of Project Settings.


#include "TLExplosiveBarrel.h"

#include "PhysicsEngine/RadialForceComponent.h"
#include "Components/StaticMeshComponent.h"
#include "DrawDebugHelpers.h"


// Sets default values
ATLExplosiveBarrel::ATLExplosiveBarrel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;

    MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
    MeshComp->SetSimulatePhysics(true);
    MeshComp->SetCollisionProfileName(UCollisionProfile::PhysicsActor_ProfileName);
    MeshComp->OnComponentHit.AddDynamic(this, &ATLExplosiveBarrel::OnActorHit);
    RootComponent = MeshComp;
    
    ForceComp = CreateDefaultSubobject<URadialForceComponent>("ForceComp");
    ForceComp->SetupAttachment(MeshComp);
    ForceComp->Radius = 1000.0f;
    ForceComp->ImpulseStrength = 110000.0f;
}

void ATLExplosiveBarrel::OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    ForceComp->FireImpulse();
}