// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BlasterGameMode.h"
#include "BuildAndDefenseGameMode.generated.h"

class ABaseAICharacter;
class ABasePlayerCharacter;
class AEnemySpawnManager;
class UCurveFloat;
class AEnemyAIControllerBase;

UCLASS()
class BLASTER_API ABuildAndDefenseGameMode : public ABlasterGameMode
{
	GENERATED_BODY()
public:
	ABuildAndDefenseGameMode();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn Wave")
		float WaveStartDelay;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn Wave")
		float SpawnInSec;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn Wave")
		int32 MaxEnemyCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn Wave")
		UCurveFloat* DifficultyCurve;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn Wave")
	TSubclassOf<AEnemyAIControllerBase> EnemyControllerClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn Wave")
		TArray<TSubclassOf<ABaseAICharacter>> EnemiesWaveToSpawn;
	UPROPERTY()
		AEnemySpawnManager* EnemySpawnManager;

	FTimerHandle SpawnTimerHandle;
	int32 EnemyAliveCount;
public:
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void Logout(AController* Exiting) override;
	virtual float CalculateDamage(AController* Attacker, AController* Victim, float BaseDamage) override;
	virtual void PlayerEliminated(ABasePlayerCharacter* ElimmedCharacter, ABlasterPlayerController* VictimController, ABlasterPlayerController* AttackerController) override;
	virtual void AIEliminated(ABaseAICharacter* ElimmedCharacter, AController* VictimController, AController* AttackerController) override;
protected:
	virtual void HandleMatchHasStarted() override;
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void StartEnemyWave();
	UFUNCTION(BlueprintCallable)
	void SpawnRandomEnemy();
};
