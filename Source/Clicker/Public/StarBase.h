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
	 * 블루프린트에서 호출 가능한 방사형 스폰 함수
	 * 레이캐스트 히트 지점에서 45도 각도로 파편을 방사형으로 소환합니다.
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

	/** 파편 비산 각도 (법선 기준, 기본 45도) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StarBase|Physics")
	float LaunchAngle = 45.f;

	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
