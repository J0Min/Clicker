// Fill out your copyright notice in the Description page of Project Settings.

#include "Public/CustomPawn.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

// Sets default values
ACustomPawn::ACustomPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	// 1. Create Capsule for collision and PHYSICS
	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	CapsuleComponent->InitCapsuleSize(35.f, 90.f);
	CapsuleComponent->SetCollisionProfileName(TEXT("Pawn"));
	
	// ENABLE PHYSICS
	CapsuleComponent->SetSimulatePhysics(true);
	CapsuleComponent->SetNotifyRigidBodyCollision(true);
	
	// Physics constraints: Don't fall over (lock X and Y rotation)
	CapsuleComponent->BodyInstance.bLockXRotation = true;
	CapsuleComponent->BodyInstance.bLockYRotation = true;
	
	// Damping to prevent infinite sliding
	CapsuleComponent->SetLinearDamping(4.0f);
	CapsuleComponent->SetAngularDamping(4.0f);
	
	RootComponent = CapsuleComponent;

	// 2. Create Camera for 1st person view
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(CapsuleComponent);
	CameraComponent->SetRelativeLocation(FVector(0.f, 0.f, 60.f)); // Position at eye level
	CameraComponent->bUsePawnControlRotation = true; // Rotate camera with controller

	
	//캡슐 컴포넌트 회전 차단
	// Pawn rotation settings
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false; // We will sync this manually in Tick for physics stability
	bUseControllerRotationRoll = false;
}

// Called when the game starts or when spawned
void ACustomPawn::BeginPlay()
{
	Super::BeginPlay();    
	UE_LOG(LogTemp, Warning, TEXT("HEEEEll"));


	// Add Input Mapping Context
	if (APlayerController* PC = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			if (DefaultMappingContext)//할당이 유효한가
			{
				Subsystem->AddMappingContext(DefaultMappingContext, 0);
			}
		}
	}
}

// Called every frame
void ACustomPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Manual Yaw Sync: Make the physics body face where the controller is looking
	if (Controller && Controller->IsLocalPlayerController())
	{
		FRotator ControlRotation = Controller->GetControlRotation();
		FRotator NewRotation = GetActorRotation();
		NewRotation.Yaw = ControlRotation.Yaw;
		
		// Set rotation directly on the physics component
		CapsuleComponent->SetWorldRotation(NewRotation);
	}
}

// Called to bind functionality to input
void ACustomPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Move
		if (MoveAction)
		{
			EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ACustomPawn::Move);
		}

		// Look
		if (LookAction)
		{
			EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ACustomPawn::Look);
		}

		// Jump
		if (JumpAction)
		{
			EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACustomPawn::Jump);
		}
	}
}

void ACustomPawn::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// Use controller yaw for direction
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// Apply physics forces instead of simple movement input
		if (CapsuleComponent)
		{
			FVector ForceToApply = (ForwardDirection * MovementVector.Y + RightDirection * MovementVector.X) * MoveForce;
			CapsuleComponent->AddForce(ForceToApply);
		}
	}
}

void ACustomPawn::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();
	UE_LOG(LogTemp, Warning, TEXT("Look Input: X=%f, Y=%f"), LookAxisVector.X, LookAxisVector.Y);


	if (Controller != nullptr)
	{
		AddControllerYawInput(LookAxisVector.X * LookSensitivity);
		AddControllerPitchInput(LookAxisVector.Y * LookSensitivity);
	}
}

void ACustomPawn::Jump()
{
	if (CapsuleComponent)
	{
		// Apply an instant physics impulse upwards
		CapsuleComponent->AddImpulse(FVector::UpVector * JumpImpulse);
	}
}
