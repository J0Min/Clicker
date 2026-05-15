// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "SpawnArea.generated.h"

UCLASS()
class CLICKER_API ASpawnArea : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASpawnArea();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** Box component defining the spawn boundary */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawning")
	class UBoxComponent* SpawnRegion;

	/** The static mesh to spawn if using SpawnNextActorWithMesh */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	TObjectPtr<UStaticMesh> MeshToSpawn;

	/** Fixed spacing between grid cells */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	float GridStepSize = 200.0f;

	/** Current indices in the grid for manual spawning */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawning|Progress")
	int32 CurrentGridX = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawning|Progress")
	int32 CurrentGridY = 0;

	/** Calculates the grid and spawns actors to fill the area densely */
	UFUNCTION(BlueprintCallable, Category = "Spawning")
	void SpawnDenseActors();

	/** Spawns the next actor as a StaticMeshActor with the specified mesh */
	UFUNCTION(BlueprintCallable, Category = "Spawning")
	bool SpawnNextActorWithMesh(UStaticMesh* InMesh = nullptr);

	/** Resets the spawning progress to the start of the area */
	UFUNCTION(BlueprintCallable, Category = "Spawning")
	void ResetSpawnProgress();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
