// Fill out your copyright notice in the Description page of Project Settings.


#include "TLItemChest.h"
#include "Net/UnrealNetwork.h"


// Sets default values
ATLItemChest::ATLItemChest()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	RootComponent = BaseMesh;

	LidMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LidMesh"));
	LidMesh->SetupAttachment(BaseMesh);

	TargetPitch = 110;

	SetReplicates(true);
}


// Called when the game starts or when spawned
void ATLItemChest::BeginPlay()
{
	Super::BeginPlay();
	
}


// Called every frame
void ATLItemChest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void ATLItemChest::OnRep_LidOpened()
{
	float CurrentPitch = bLidOpened ? TargetPitch : 0.0f;
	LidMesh->SetRelativeRotation(FRotator(CurrentPitch, 0, 0));
}


void ATLItemChest::Interact_Implementation(APawn* InstigatorPawn, ETLInteractionType InteractionTypeUsed)
{
	if (InteractionTypeUsed == ETLInteractionType::EBT_NEARBY)
	{
		bLidOpened = !bLidOpened;
		OnRep_LidOpened();
		//InteractionSuccessful = true;
		return;
	}
	//InteractionSuccessful = false;
}


void ATLItemChest::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATLItemChest, bLidOpened);
}