// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemySpawnManager.generated.h"

UCLASS()
class BLASTER_API AEnemySpawnManager : public AActor
{
	GENERATED_BODY()
	
public:	
	AEnemySpawnManager();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn Wave")
	TArray<class ATargetPoint*> SpawnPoints;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn Wave")
	TArray<class ATargetPoint*> TargetPoints;
protected:
	virtual void BeginPlay() override;

public:	
	UFUNCTION()
	FTransform GetRandomSpawnTransform();
	UFUNCTION()
	FVector GetTargetLocation();
	//virtual void Tick(float DeltaTime) override;

};
