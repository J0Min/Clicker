// Fill out your copyright notice in the Description page of Project Settings.


#include "Ham.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AHam::AHam()
{
	PrimaryActorTick.bCanEverTick = true;

	HammerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HammerMesh"));
	RootComponent = HammerMesh;
}

// Called when the game starts or when spawned
void AHam::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AHam::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CurrentState == EHamState::Striking)
	{
		FRotator CurrentRotation = GetActorRotation();
		FRotator NextRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaTime, RotationSpeed);
		SetActorRotation(NextRotation);

		// Check if we reached the hit angle
		if (FMath::Abs(NextRotation.Pitch - TargetRotation.Pitch) < 1.0f)
		{
			CurrentState = EHamState::Returning;
		}
	}
	else if (CurrentState == EHamState::Returning)
	{
		FRotator CurrentRotation = GetActorRotation();
		FRotator NextRotation = FMath::RInterpTo(CurrentRotation, RaisedRotation, DeltaTime, RotationSpeed * 0.5f);
		SetActorRotation(NextRotation);

		// Check if we returned to raised angle
		if (FMath::Abs(NextRotation.Pitch - RaisedRotation.Pitch) < 1.0f)
		{
			CurrentState = EHamState::Idle;
		}
	}
}

void AHam::StartStrike(AActor* TargetActor, AActor* StrikerCharacter)
{
	if (CurrentState != EHamState::Idle || !TargetActor || !StrikerCharacter)
	{
		return;
	}

	FVector TargetPos = TargetActor->GetActorLocation();
	FVector CharacterForward = StrikerCharacter->GetActorForwardVector();
	CharacterForward.Z = 0.0f;
	CharacterForward.Normalize();

	// The actor itself is the pivot.
	// Position the pivot behind the target relative to character's facing.
	FVector PivotLocation = TargetPos - (CharacterForward * PivotDistance);
	
	// Set the height of the pivot (usually same as target height or slightly adjusted)
	PivotLocation.Z = TargetPos.Z; 

	SetActorLocation(PivotLocation);

	// Base rotation is facing the target
	BaseRotation = CharacterForward.Rotation();
	
	// Define the start (raised) and end (hit) rotations
	RaisedRotation = BaseRotation + FRotator(RaisedAngle, 0, 0);
	TargetRotation = BaseRotation + FRotator(HitAngle, 0, 0);

	// Start from the raised position
	SetActorRotation(RaisedRotation);

	CurrentState = EHamState::Striking;
}
/* ⚠️ 에디터에서 꼭 설정해야 할 것 (중요)

이 로직이 기둥을 잡고 흔드는 것처럼 보이려면 블루프린트에서 메쉬의 위치를 맞춰야 합니다.

1. BP_Hammer를 엽니다.
2. HammerMesh 컴포넌트를 선택합니다.
3. 망치의 손잡이 끝부분(축이 될 부분)이 블루프린트의 중심점(0, 0, 0)에 오도록 메쉬를 이동시킵니다.
* 보통 X축이나 Y축으로 메쉬를 밀어서 손잡이 끝이 중앙에 오게 합니다.
4. Pivot Distance 값을 망치 자루의 실제 길이에 맞춰 조절합니다 (기본값 200).
5. Raised Angle을 조절하여 망치를 얼마나 높이 들어 올릴지 정합니다 (예: -70도).

이렇게 설정하면 망치 액터 자체는 제자리에 있고, 머리 부분만 기둥을 축으로 삼아 휘둘러지는 멋진 떡방아 동작이
완성됩니다! 컴파일 후 바로 확인해 보세요.
*/