// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyAIControllerBase.generated.h"

UCLASS()
class BLASTER_API AEnemyAIControllerBase : public AAIController
{
	GENERATED_BODY()
protected:
	UPROPERTY(BlueprintReadOnly)
	class AEnemySpawnManager* EnemySpawnManager;
	UPROPERTY(EditDefaultsOnly)
	class UBehaviorTree* BehaviorTree;
protected:
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;
	//virtual void OnUnPossess() override;
};
