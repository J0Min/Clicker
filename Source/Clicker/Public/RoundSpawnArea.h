// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RoundSpawnArea.generated.h"

UCLASS()
class CLICKER_API ARoundSpawnArea : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARoundSpawnArea();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** 중앙에 위치할 메쉬 컴포넌트 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UStaticMeshComponent> CenterMesh;

public:	
	/** 주변 액터들을 다시 생성하고 배치하는 함수 (매개변수로 직접 제어 가능) */
	UFUNCTION(BlueprintCallable, Category = "SpawnArea")
	void ClearAndSpawn(float InRadius = 300.f, int32 InSpawnCount = 5, TSubclassOf<AActor> InActorClass = nullptr);

	/** 원형 배치의 기본 반경 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnArea|Defaults")
	float Radius = 300.f;

	/** 생성할 주변 액터의 기본 개수 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnArea|Defaults")
	int32 SpawnCount = 5;

	/** 주변에 소환할 기본 액터 클래스 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnArea|Defaults")
	TSubclassOf<AActor> PeripheralActorClass;

	/** 중앙을 바라볼 때의 회전 오프셋 (모델링 방향에 따라 조절) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnArea|Settings")
	FRotator RotationOffset = FRotator::ZeroRotator;

private:
	/** 현재 생성된 액터들을 추적하기 위한 배열 */
	UPROPERTY()
	TArray<TObjectPtr<AActor>> SpawnedActors;

	/** 기존 액터들을 제거하는 내부 함수 */
	void ClearSpawnedActors();
};
