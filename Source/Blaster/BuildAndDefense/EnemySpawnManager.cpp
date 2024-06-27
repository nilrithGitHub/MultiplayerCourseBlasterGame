// Fill out your copyright notice in the Description page of Project Settings.


#include "Blaster/BuildAndDefense/EnemySpawnManager.h"
#include "Engine/TargetPoint.h"

// Sets default values
AEnemySpawnManager::AEnemySpawnManager()
{
	PrimaryActorTick.bCanEverTick = false;

}
void AEnemySpawnManager::BeginPlay()
{
	Super::BeginPlay();
	
}
FTransform AEnemySpawnManager::GetRandomSpawnTransform()
{
	int32 NumSpawnPoint = SpawnPoints.Num();
	
	if (NumSpawnPoint > 0)
	{
		int32 Selection = FMath::RandRange(0, NumSpawnPoint - 1);
		ATargetPoint* TargetPoint = SpawnPoints[Selection];
		if (TargetPoint)
		{
			FVector SpawnLocation = TargetPoint->GetActorLocation();
			FRotator SpawnRotator = TargetPoint->GetActorRotation();
			FVector SpawnScale = TargetPoint->GetActorScale3D();
			return FTransform (SpawnRotator, SpawnLocation, SpawnScale);
		}
	}
	return FTransform ();
}

FVector AEnemySpawnManager::GetTargetLocation()
{
	int32 NumSpawnPoint = TargetPoints.Num();
	if (NumSpawnPoint > 0)
	{
		int32 Selection = FMath::RandRange(0, NumSpawnPoint - 1);
		ATargetPoint* TargetPoint = TargetPoints[Selection];
		if (TargetPoint)
		{
			return TargetPoint->GetActorLocation();
		}
	}
	return FVector();
}

