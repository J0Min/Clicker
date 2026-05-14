// Fill out your copyright notice in the Description page of Project Settings.

#include "Public/SpawnArea.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
ASpawnArea::ASpawnArea()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Initialize the spawn region box component
	SpawnRegion = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawnRegion"));
	RootComponent = SpawnRegion;
	
	// Default box size
	SpawnRegion->SetBoxExtent(FVector(500.f, 500.f, 10.f));
}

// Called when the game starts or when spawned
void ASpawnArea::BeginPlay()
{
	Super::BeginPlay();
	
	SpawnDenseActors();
}

// Called every frame
void ASpawnArea::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASpawnArea::SpawnDenseActors()
{
	if (!ActorToSpawn)
	{
		UE_LOG(LogTemp, Warning, TEXT("SpawnArea: ActorToSpawn is not set."));
		return;
	}

	UWorld* World = GetWorld();
	if (!World) return;

	// 1. Get the area dimensions
	FVector BoxExtent = SpawnRegion->GetScaledBoxExtent();
	FVector BoxOrigin = GetActorLocation();

	// 2. Determine actor size for spacing
	// We use the CDO (Class Default Object) to estimate size without spawning first
	AActor* DefaultActor = ActorToSpawn->GetDefaultObject<AActor>();
	FVector Origin, ActorExtent;
	DefaultActor->GetActorBounds(true, Origin, ActorExtent);

	// Spacing based on actor extent (full width/length) plus padding
	float StepX = (ActorExtent.X * 2.0f) + SpacingPadding;
	float StepY = (ActorExtent.Y * 2.0f) + SpacingPadding;

	// Ensure we don't divide by zero if the actor has no bounds
	if (StepX <= 0.0f) StepX = 100.0f;
	if (StepY <= 0.0f) StepY = 100.0f;

	// 3. Grid Spawning Loop
	// Calculate start position (bottom-left of the box)
	// We start half a step in to center the grid or at least keep actors inside
	FVector StartPos = BoxOrigin - BoxExtent + FVector(ActorExtent.X, ActorExtent.Y, BoxExtent.Z);

	for (float x = 0; x <= (BoxExtent.X * 2.0f) - (ActorExtent.X * 2.0f); x += StepX)
	{
		for (float y = 0; y <= (BoxExtent.Y * 2.0f) - (ActorExtent.Y * 2.0f); y += StepY)
		{
			FVector SpawnLocation = StartPos + FVector(x, y, 0.0f);
			FRotator SpawnRotation = GetActorRotation();

			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = GetInstigator();
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

			World->SpawnActor<AActor>(ActorToSpawn, SpawnLocation, SpawnRotation, SpawnParams);
		}
	}
}
