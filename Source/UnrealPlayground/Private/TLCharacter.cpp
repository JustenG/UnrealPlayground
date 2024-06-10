// Fill out your copyright notice in the Description page of Project Settings.


#include "TLCharacter.h"

#include "Camera/CameraComponent.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "TLInteractionComponent.h"
#include "TLAttributeComponent.h"


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

	AtributeComp = CreateDefaultSubobject<UTLAttributeComponent>("AtributeComp");

	GetCharacterMovement()->bOrientRotationToMovement = true; // Face character model in direction the character is moving

	bUseControllerRotationYaw = false; // Don't rotate the character based on Mouse X
}


// Called when the game starts or when spawned
void ATLCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}


// Called every frame
void ATLCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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


// Called to bind functionality to input
void ATLCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ATLCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ATLCharacter::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &ATLCharacter::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &ATLCharacter::AddControllerPitchInput);

	PlayerInputComponent->BindAction("PrimaryAttack", IE_Pressed, this, &ATLCharacter::PrimaryAttack);
	PlayerInputComponent->BindAction("PrimaryInteract", IE_Pressed, this, &ATLCharacter::PrimaryInteract);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ATLCharacter::Jump);
	PlayerInputComponent->BindAction("SecondaryAttack", IE_Pressed, this, &ATLCharacter::SecondaryAttack);
	PlayerInputComponent->BindAction("UltimateAttack", IE_Pressed, this, &ATLCharacter::UltimateAttack);
	
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

void ATLCharacter::PrimaryAttack()
{
	if (!GetWorldTimerManager().IsTimerActive(TimerHandle_SpawnAttack))
	{
		PlayAnimMontage(AttackAnim);
		GetWorldTimerManager().SetTimer(TimerHandle_SpawnAttack, this, &ATLCharacter::SpawnAttack_TimeElapsed, 0.2f);
		LastRequestProjectileClass = ProjectileClassPrimary;
	}
}


void ATLCharacter::SecondaryAttack()
{
	if (!GetWorldTimerManager().IsTimerActive(TimerHandle_SpawnAttack))
	{
		PlayAnimMontage(AttackAnim);
		GetWorldTimerManager().SetTimer(TimerHandle_SpawnAttack, this, &ATLCharacter::SpawnAttack_TimeElapsed, 0.2f);
		LastRequestProjectileClass = ProjectileClassSecondary;
	}
}


void ATLCharacter::UltimateAttack()
{
	if (!GetWorldTimerManager().IsTimerActive(TimerHandle_SpawnAttack))
	{
		PlayAnimMontage(AttackAnim);
		GetWorldTimerManager().SetTimer(TimerHandle_SpawnAttack, this, &ATLCharacter::SpawnAttack_TimeElapsed, 0.2f);
		LastRequestProjectileClass = ProjectileClassUltimate;
	}
}


void ATLCharacter::SpawnAttack_TimeElapsed()
{
	SpawnAttack(LastRequestProjectileClass);
}


void ATLCharacter::SpawnAttack(TSubclassOf<AActor>& ProjectileClass)
{
	FVector HandLocation = GetMesh()->GetSocketLocation("Muzzle_01");

	FVector Start = CameraComp->GetComponentLocation();
	FVector Forward = CameraComp->GetForwardVector();
	FVector End = Start + (Forward * 10000);

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
	SpawnParams.Instigator = this;

	GetWorld()->SpawnActor<AActor>(ProjectileClass, PrimaryAttackSpawnTM, SpawnParams);

	GetWorldTimerManager().ClearTimer(TimerHandle_SpawnAttack);
}


void ATLCharacter::PrimaryInteract()
{
	InteractionComp->PrimaryInteract();
}
