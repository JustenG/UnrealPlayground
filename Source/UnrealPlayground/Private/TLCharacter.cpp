// Fill out your copyright notice in the Description page of Project Settings.


#include "TLCharacter.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TLInteractionComponent.h"
#include "TLAttributeComponent.h"
#include "TLGameplayInterface.h"
#include "TLActionComponent.h"


static TAutoConsoleVariable<bool> CVarDebugDrawPlayerDir(TEXT("tl.DebugDraw.PlayerDir"), false, TEXT("Enable Debug Arrows for Player Direction."), ECVF_Cheat);


// Sets default values
ATLCharacter::ATLCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArmComp");
	SpringArmComp->bUsePawnControlRotation = true; // Allows the spring arm to move in the Y Axis based on player mouse Y input movement
	SpringArmComp->SetupAttachment(RootComponent);

	CameraComp = CreateDefaultSubobject<UCameraComponent>( "CameraComp" );
	CameraComp->SetupAttachment(SpringArmComp);

	InteractionComp = CreateDefaultSubobject<UTLInteractionComponent>("InteractionComp");

	AttributeComp = CreateDefaultSubobject<UTLAttributeComponent>("AtributeComp");

	ActionComp = CreateDefaultSubobject<UTLActionComponent>("ActionComp");

	GetCharacterMovement()->bOrientRotationToMovement = true; // Face character model in direction the character is moving

	bUseControllerRotationYaw = false; // Don't rotate the character based on Mouse X
	TimeToHitParamName = "TimeToHit";
	AttackSpawnBoneParamName = "Muzzle_01";
}


// Called when the game starts or when spawned
void ATLCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

FVector ATLCharacter::GetPawnViewLocation() const
{
	return CameraComp->GetComponentLocation();
}


// Called every frame
void ATLCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CVarDebugDrawPlayerDir.GetValueOnGameThread())
	{
		// -- Rotation Visualization -- //
		const float DrawScale = 100.0f;
		const float Thickness = 5.0f;

		FVector LineStart = GetActorLocation();
		// Offset to the right of pawn
		LineStart += GetActorRightVector() * 100.0f;
		// Set line end in direction of the actor's forward
		FVector ActorDirection_LineEnd = LineStart + (GetActorForwardVector() * 100.0f);
		// Draw Actor's Direction
		DrawDebugDirectionalArrow(GetWorld(), LineStart, ActorDirection_LineEnd, DrawScale, FColor::Yellow, false, 0.0f, 0, Thickness);

		FVector ControllerDirection_LineEnd = LineStart + (GetControlRotation().Vector() * 100.0f);
		// Draw 'Controller' Rotation ('PlayerController' that 'possessed' this character)
		DrawDebugDirectionalArrow(GetWorld(), LineStart, ControllerDirection_LineEnd, DrawScale, FColor::Green, false, 0.0f, 0, Thickness);
	}

}


// Called to bind functionality to input
void ATLCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ATLCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ATLCharacter::MoveRight);

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &ATLCharacter::SprintStart);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &ATLCharacter::SprintStop);

	PlayerInputComponent->BindAxis("Turn", this, &ATLCharacter::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &ATLCharacter::AddControllerPitchInput);

	PlayerInputComponent->BindAction("PrimaryAttack", IE_Pressed, this, &ATLCharacter::PrimaryAttack);
	PlayerInputComponent->BindAction("PrimaryInteract", IE_Pressed, this, &ATLCharacter::PrimaryInteract);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ATLCharacter::Jump);
	PlayerInputComponent->BindAction("SecondaryAttack", IE_Pressed, this, &ATLCharacter::SecondaryAttack);
	PlayerInputComponent->BindAction("UltimateAttack", IE_Pressed, this, &ATLCharacter::UltimateAttack);
}

void ATLCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	AttributeComp->OnHealthChanged.AddDynamic(this, &ATLCharacter::OnHealthChanged);
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &ATLCharacter::OnActorOverlap);
}


void ATLCharacter::MoveForward(float Value)
{
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.0f;
	ControlRot.Roll = 0.0f;

	AddMovementInput(ControlRot.Vector(), Value);
}


void ATLCharacter::MoveRight(float Value)
{
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.0f;
	ControlRot.Roll = 0.0f;

	// X Forward, Y Right, Z Up
	FVector RightVector = FRotationMatrix(ControlRot).GetScaledAxis(EAxis::Y);

	AddMovementInput(RightVector, Value);
}


void ATLCharacter::SprintStart()
{
	ActionComp->StartActionByName(this, "Sprint");
}


void ATLCharacter::SprintStop()
{
	ActionComp->StopActionByName(this, "Sprint");
}


void ATLCharacter::PrimaryAttack()
{
	ActionComp->StartActionByName(this, "PrimaryAttack");
}


void ATLCharacter::SecondaryAttack()
{
	ActionComp->StartActionByName(this, "Dash");
}


void ATLCharacter::UltimateAttack()
{
	ActionComp->StartActionByName(this, "Blackhole");
}


void ATLCharacter::PrimaryInteract()
{
	// Direct Camera Interaction 
	FVector Start = CameraComp->GetComponentLocation();
	FVector Forward = CameraComp->GetForwardVector();
	FVector End = Start + (Forward * 1000);

	InteractionComp->InteractDirect(Start, End);

	// Fallback to Nearby interaction
	InteractionComp->InteractNearby();
}


void ATLCharacter::OnHealthChanged(AActor* InstigatorActor, UTLAttributeComponent* OwningComp, float NewHealth, float Delta)
{
	if (Delta < 0.0f)
	{
		GetMesh()->SetScalarParameterValueOnMaterials(TimeToHitParamName, static_cast<float>(GetWorld()->TimeSeconds));

		AttributeComp->ApplyRageChange(InstigatorActor, FMath::Abs(Delta));
	}

	// Died
	if (NewHealth <= 0.0f && Delta < 0.0f)
	{
		APlayerController* PC = GetController<APlayerController>();
		DisableInput(PC);

		SetLifeSpan(5.0f);
	}
}

void ATLCharacter::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->Implements<UTLGameplayInterface>())
	{
		//ITLGameplayInterface::Execute_Interact(OtherActor, this, ETLInteractionType::EBT_CONTACT);
		InteractionComp->ServerInteract(OtherActor, ETLInteractionType::EBT_CONTACT);
	}
}


void ATLCharacter::HealSelf(float Amount  /* = 0 */)
{
	if (Amount == 0)
	{
		Amount = AttributeComp->GetHealthMax();
	}
	AttributeComp->ApplyHealthChange(this, Amount);
}