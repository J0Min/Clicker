// Fill out your copyright notice in the Description page of Project Settings.


#include "Total.h"


// Sets default values
ATotal::ATotal()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ATotal::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATotal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

