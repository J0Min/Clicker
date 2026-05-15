// Fill out your copyright notice in the Description page of Project Settings.

#include "Public/SpawnArea.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Engine/StaticMeshActor.h"
#include "Components/StaticMeshComponent.h"

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
}

// Called every frame
void ASpawnArea::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASpawnArea::ResetSpawnProgress()
{
	CurrentGridX = 0;
	CurrentGridY = 0;
	UE_LOG(LogTemp, Display, TEXT("SpawnArea: Progress reset."));
}

bool ASpawnArea::SpawnNextActorWithMesh(UStaticMesh* InMesh)
{
	UStaticMesh* MeshToUse = InMesh ? InMesh : MeshToSpawn.Get();

	if (!MeshToUse)
	{
		UE_LOG(LogTemp, Warning, TEXT("SpawnArea: No mesh provided for spawning."));
		return false;
	}

	UWorld* World = GetWorld();
	if (!World) return false;

	// 1. Get dimensions
	FVector BoxExtent = SpawnRegion->GetScaledBoxExtent();
	FVector BoxOrigin = GetActorLocation();

	// 2. Fixed spacing logic
	float StepX = GridStepSize;
	float StepY = GridStepSize;

	if (StepX <= 0.0f) StepX = 100.0f;
	if (StepY <= 0.0f) StepY = 100.0f;

	// 3. Calculate max counts based on Box Extent
	// We use StepX/2 as a small offset to keep things within bounds
	int32 MaxX = FMath::FloorToInt((BoxExtent.X * 2.0f) / StepX) - 1;
	int32 MaxY = FMath::FloorToInt((BoxExtent.Y * 2.0f) / StepY) - 1;

	if (CurrentGridY > MaxY)
	{
		UE_LOG(LogTemp, Warning, TEXT("SpawnArea: Area is already full."));
		return false;
	}

	// 4. Calculate Location (Centered within the box relative to top-left/bottom-left)
	// Start from the corner and move by half-step to center the first cell
	FVector StartPos = BoxOrigin - BoxExtent + FVector(StepX * 0.5f, StepY * 0.5f, BoxExtent.Z);
	FVector SpawnLocation = StartPos + FVector(CurrentGridX * StepX, CurrentGridY * StepY, 0.0f);
	FRotator SpawnRotation = GetActorRotation();

	// 5. Spawn StaticMeshActor
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	AStaticMeshActor* NewActor = World->SpawnActor<AStaticMeshActor>(AStaticMeshActor::StaticClass(), SpawnLocation, SpawnRotation, SpawnParams);

	if (NewActor)
	{
		NewActor->GetStaticMeshComponent()->SetStaticMesh(MeshToUse);
		NewActor->SetMobility(EComponentMobility::Movable);

		// 6. Increment indices
		CurrentGridX++;
		if (CurrentGridX > MaxX)
		{
			CurrentGridX = 0;
			CurrentGridY++;
		}
		return true;
	}

	return false;
}

void ASpawnArea::SpawnDenseActors()
{
	UStaticMesh* MeshToUse = MeshToSpawn.Get();
	if (!MeshToUse)
	{
		UE_LOG(LogTemp, Warning, TEXT("SpawnArea: MeshToSpawn is not set."));
		return;
	}

	UWorld* World = GetWorld();
	if (!World) return;

	FVector BoxExtent = SpawnRegion->GetScaledBoxExtent();
	FVector BoxOrigin = GetActorLocation();

	float StepX = GridStepSize;
	float StepY = GridStepSize;

	if (StepX <= 0.0f) StepX = 100.0f;
	if (StepY <= 0.0f) StepY = 100.0f;

	FVector StartPos = BoxOrigin - BoxExtent + FVector(StepX * 0.5f, StepY * 0.5f, BoxExtent.Z);

	for (float x = 0; x <= (BoxExtent.X * 2.0f) - StepX; x += StepX)
	{
		for (float y = 0; y <= (BoxExtent.Y * 2.0f) - StepY; y += StepY)
		{
			FVector SpawnLocation = StartPos + FVector(x, y, 0.0f);
			FRotator SpawnRotation = GetActorRotation();

			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = GetInstigator();
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

			AStaticMeshActor* NewActor = World->SpawnActor<AStaticMeshActor>(AStaticMeshActor::StaticClass(), SpawnLocation, SpawnRotation, SpawnParams);
			if (NewActor)
			{
				NewActor->GetStaticMeshComponent()->SetStaticMesh(MeshToUse);
				NewActor->SetMobility(EComponentMobility::Movable);
			}
		}
	}
}
