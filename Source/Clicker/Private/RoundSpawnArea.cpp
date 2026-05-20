// Fill out your copyright notice in the Description page of Project Settings.

#include "RoundSpawnArea.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ARoundSpawnArea::ARoundSpawnArea()
{
	PrimaryActorTick.bCanEverTick = false;

	// 중앙 메쉬 생성 및 루트로 설정
	CenterMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CenterMesh"));
	RootComponent = CenterMesh;
}

// Called when the game starts or when spawned
void ARoundSpawnArea::BeginPlay()
{
	Super::BeginPlay();
	
	// 게임 시작 시 초기 설정된 값으로 배치 실행
	ClearAndSpawn(Radius, SpawnCount, PeripheralActorClass);
}

void ARoundSpawnArea::ClearAndSpawn(float InRadius, int32 InSpawnCount, TSubclassOf<AActor> InActorClass)
{
	UWorld* World = GetWorld();
	
	// 매개변수로 클래스가 들어오지 않으면 기본 설정된 클래스 사용
	TSubclassOf<AActor> FinalClass = InActorClass ? InActorClass : PeripheralActorClass;
	
	if (!World || !FinalClass) return;

	// 1. 기존 액터 제거
	ClearSpawnedActors();

	// 2. 새로운 액터 배치
	for (int32 i = 0; i < InSpawnCount; ++i)
	{
		// 각도 계산 (360도 / 개수)
		float Angle = (360.0f / InSpawnCount) * i;
		float AngleRad = FMath::DegreesToRadians(Angle);

		// 원형 좌표 계산 (X = Cos, Y = Sin)
		FVector RelativeLocation = FVector(
			FMath::Cos(AngleRad) * InRadius,
			FMath::Sin(AngleRad) * InRadius,
			0.0f
		);

		FVector SpawnLocation = GetActorLocation() + RelativeLocation;

		// 중앙을 바라보는 회전값 계산
		FRotator LookAtRot = UKismetMathLibrary::FindLookAtRotation(SpawnLocation, GetActorLocation());
		
		// 오프셋 적용
		LookAtRot += RotationOffset;

		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		if (AActor* NewActor = World->SpawnActor<AActor>(FinalClass, SpawnLocation, LookAtRot, SpawnParams))
		{
			SpawnedActors.Add(NewActor);
		}
	}
}

void ARoundSpawnArea::ClearSpawnedActors()
{
	for (AActor* Actor : SpawnedActors)
	{
		if (Actor)
		{
			Actor->Destroy();
		}
	}
	SpawnedActors.Empty();
}
