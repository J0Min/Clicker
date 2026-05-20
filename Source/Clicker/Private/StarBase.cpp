// Fill out your copyright notice in the Description page of Project Settings.

#include "StarBase.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AStarBase::AStarBase()
{
	PrimaryActorTick.bCanEverTick = true;

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	RootComponent = BoxCollision;

	BoxCollision->SetBoxExtent(FVector(500.f, 500.f, 500.f));
	BoxCollision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	BaseMesh->SetupAttachment(RootComponent);

	// 외형 메쉬가 파편의 스폰과 비산을 방해하지 않도록 콜리전을 비활성화합니다.
	BaseMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AStarBase::BeginPlay()
{
	Super::BeginPlay();

	// 초기 설정값으로 주변 액터 배치
	if (PeripheralActorClass)
	{
		ClearAndSpawn(Radius, SpawnCount, PeripheralActorClass);
	}
}

void AStarBase::ClearAndSpawn(float InRadius, int32 InSpawnCount, TSubclassOf<AActor> InActorClass)
{
	UWorld* World = GetWorld();
	TSubclassOf<AActor> FinalClass = InActorClass ? InActorClass : PeripheralActorClass;

	if (!World || !FinalClass) return;

	// 1. 기존 액터 제거
	ClearSpawnedActors();

	// 2. 새로운 액터 배치
	for (int32 i = 0; i < InSpawnCount; ++i)
	{
		float Angle = (360.0f / InSpawnCount) * i;
		float AngleRad = FMath::DegreesToRadians(Angle);

		FVector RelativeLocation = FVector(
			FMath::Cos(AngleRad) * InRadius,
			FMath::Sin(AngleRad) * InRadius,
			0.0f
		);

		FVector SpawnLocation = GetActorLocation() + RelativeLocation;

		// 중앙을 바라보는 회전값 계산
		FRotator LookAtRot = UKismetMathLibrary::FindLookAtRotation(SpawnLocation, GetActorLocation());
		LookAtRot += RotationOffset;

		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		if (AActor* NewActor = World->SpawnActor<AActor>(FinalClass, SpawnLocation, LookAtRot, SpawnParams))
		{
			// StarBase와의 충돌 무시
			if (BoxCollision) BoxCollision->IgnoreActorWhenMoving(NewActor, true);
			if (BaseMesh) BaseMesh->IgnoreActorWhenMoving(NewActor, true);

			SpawnedActors.Add(NewActor);
		}
	}
}

void AStarBase::ClearSpawnedActors()
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


void AStarBase::TriggerRadialSpawn(FVector HitLocation, FVector HitNormal, TSubclassOf<AActor> ActorClass, int32 ActorCount, UNiagaraSystem* Effect, float ImpulseStrength, float InLaunchAngle)
{
	UWorld* World = GetWorld();
	if (!World || !ActorClass) return;

	// 1. 나이아가라 이펙트 소환
	if (Effect)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(World, Effect, HitLocation, HitNormal.Rotation());
	}

	// 2. 법선 벡터(HitNormal)가 유효하지 않을 경우 위쪽 방향으로 기본 설정
	if (HitNormal.IsNearlyZero())
	{
		HitNormal = FVector::UpVector;
	}
// 3. 법선에 수직인 기준 축(Tangent) 계산
FVector TangentX, TangentY;
HitNormal.FindBestAxisVectors(TangentX, TangentY);

// 시작 각도를 무작위로 설정하여 1개만 생성해도 매번 다른 방향으로 튀게 함
float BaseRandomRotation = FMath::FRandRange(0.0f, 360.0f);

for (int32 i = 0; i < ActorCount; ++i)
{
	// 겹침 방지를 위해 표면에서 아주 살짝 띄움
	FVector SpawnLocation = HitLocation + (HitNormal * 5.0f);

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	if (AActor* SpawnedActor = World->SpawnActor<AActor>(ActorClass, SpawnLocation, FRotator::ZeroRotator, SpawnParams))
	{
		// StarBase 컴포넌트들과의 충돌 무시 설정
		if (BoxCollision) BoxCollision->IgnoreActorWhenMoving(SpawnedActor, true);
		if (BaseMesh) BaseMesh->IgnoreActorWhenMoving(SpawnedActor, true);

		// --- 방사형 벡터 계산 ---
		// 1. 법선을 매개변수로 받은 InLaunchAngle만큼 기울임 (TangentX를 축으로 회전)
		FVector LaunchDirection = HitNormal.RotateAngleAxis(InLaunchAngle, TangentX);

		// 2. 기울어진 벡터를 법선(HitNormal) 축을 중심으로 360도 회전
		// BaseRandomRotation을 더해 시작 위치를 무작위화함
		float CurrentRotation = BaseRandomRotation + (360.0f / ActorCount) * i;
		LaunchDirection = LaunchDirection.RotateAngleAxis(CurrentRotation, HitNormal);

		FVector FinalImpulse = LaunchDirection * ImpulseStrength;

			// 약간의 힘 차이 부여
			FinalImpulse *= FMath::FRandRange(0.8f, 1.2f);

			TArray<UPrimitiveComponent*> PhysicsComponents;
			SpawnedActor->GetComponents<UPrimitiveComponent>(PhysicsComponents);

			for (UPrimitiveComponent* Prim : PhysicsComponents)
			{
				if (Prim)
				{
					Prim->IgnoreActorWhenMoving(this, true);
					Prim->SetSimulatePhysics(true);
					Prim->AddImpulse(FinalImpulse, NAME_None, true);
				}
			}
		}
	}
}

void AStarBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
