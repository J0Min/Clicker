// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "StarBase.generated.h"

class UBoxComponent;
class UStaticMeshComponent;
class UNiagaraSystem;

UCLASS()
class CLICKER_API AStarBase : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AStarBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** 루트 컴포넌트로 사용할 박스 콜리전 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UBoxComponent> BoxCollision;

	/** 외형을 담당할 스태틱 메쉬 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UStaticMeshComponent> BaseMesh;

public:
	/** 
	 * 블루프린트에서 호출 가능한 방사형 스폰 함수 (1회성 버스트)
	 */
	UFUNCTION(BlueprintCallable, Category = "StarBase|Events")
	void TriggerRadialSpawn(
		FVector HitLocation, 
		FVector HitNormal, 
		TSubclassOf<AActor> ActorClass, 
		int32 ActorCount, 
		UNiagaraSystem* Effect, 
		float ImpulseStrength = 1500.f,
		float InLaunchAngle = 45.f
	);

	/** 주변 액터들을 원형으로 배치하는 함수 (지속적 배치) */
	UFUNCTION(BlueprintCallable, Category = "StarBase|Events")
	void ClearAndSpawn(float InRadius = 300.f, int32 InSpawnCount = 5, TSubclassOf<AActor> InActorClass = nullptr, FRotator InRotationOffset = FRotator::ZeroRotator);

	/** 파편 비산 각도 (법선 기준, 기본 45도) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StarBase|Physics")
	float LaunchAngle = 45.f;

	/** 원형 배치의 기본 반경 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StarBase|Circular")
	float Radius = 300.f;

	/** 생성할 주변 액터의 기본 개수 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StarBase|Circular")
	int32 SpawnCount = 5;

	/** 주변에 소환할 기본 액터 클래스 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StarBase|Circular")
	TSubclassOf<AActor> PeripheralActorClass;

	/** 중앙을 바라볼 때의 회전 오프셋 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StarBase|Circular")
	FRotator RotationOffset = FRotator::ZeroRotator;

private:
	/** 지속적으로 생성된 액터들을 추적하기 위한 배열 */
	UPROPERTY()
	TArray<TObjectPtr<AActor>> SpawnedActors;

	/** 기존 액터들을 제거하는 내부 함수 */
	void ClearSpawnedActors();

	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
