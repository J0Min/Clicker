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

	/** The class of the actor to spawn */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	TSubclassOf<AActor> ActorToSpawn;

	/** Additional padding between actors in the grid */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	float SpacingPadding = 0.0f;

	/** Calculates the grid and spawns actors to fill the area densely */
	UFUNCTION(BlueprintCallable, Category = "Spawning")
	void SpawnDenseActors();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
