// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Ham.generated.h"

UENUM(BlueprintType)
enum class EHamState : uint8
{
	Idle,
	Striking,
	Returning
};

UCLASS()
class CLICKER_API AHam : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AHam();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/** Starts the hammer swing sequence */
	UFUNCTION(BlueprintCallable, Category = "Hammer")
	void StartStrike(AActor* TargetActor, AActor* StrikerCharacter);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* HammerMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hammer Settings")
	float RotationSpeed = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hammer Settings")
	float PivotDistance = 200.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hammer Settings")
	float RaisedAngle = -70.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hammer Settings")
	float HitAngle = 0.0f;

private:
	EHamState CurrentState = EHamState::Idle;
	FRotator BaseRotation;
	FRotator TargetRotation;
	FRotator RaisedRotation;
};
