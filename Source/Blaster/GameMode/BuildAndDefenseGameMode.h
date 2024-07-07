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

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnWaveDifficultyUpdated, float, NewDifficulty, float, OldDifficulty);

UCLASS()
class BLASTER_API ABuildAndDefenseGameMode : public ABlasterGameMode
{
	GENERATED_BODY()
public:
	ABuildAndDefenseGameMode();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave")
		float WaveStartDelay;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave")
		float WaveTickSec;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave")
		UCurveFloat* DifficultyCurve;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave")
		UCurveFloat* MaxSpawnCountDifficultyCurve;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave")
		UCurveFloat* AILevelDifficultyCurve;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave")
		UCurveFloat* SpawnTickDifficultyCurve;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave")
		TSubclassOf<AEnemyAIControllerBase> EnemyControllerClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave")
		TSubclassOf<ABaseAICharacter> TestEnemyWaveToSpawn;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave")
		TArray<TSubclassOf<ABaseAICharacter>> EnemiesWaveToSpawn;
	UPROPERTY(BlueprintReadOnly, Category = "Wave")
		float CurrentDifficulty;
	UPROPERTY(BlueprintReadOnly, Category = "Wave")
		AEnemySpawnManager* EnemySpawnManager;

	FTimerHandle WaveTimerHandle;
	FTimerHandle SpawnEnemyTimerHandle;

	UPROPERTY(BlueprintReadOnly, Category = "Wave")
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
	UFUNCTION()
	void WaveTick();
	UFUNCTION()
	void UpdateCurrentDifficulty(float DifficultyToUpdate);

public:
	UPROPERTY(BlueprintAssignable, Category = "Wave")
	FOnWaveDifficultyUpdated OnWaveDifficultyUpdated;

	FORCEINLINE float GetMaxEnemyCount_Difficulty () { return MaxSpawnCountDifficultyCurve ? MaxSpawnCountDifficultyCurve->GetFloatValue(CurrentDifficulty) : 1; }
	FORCEINLINE float GetAILevel_Difficulty() { return AILevelDifficultyCurve ? AILevelDifficultyCurve->GetFloatValue(CurrentDifficulty) : 1; }
	FORCEINLINE float GetSpawnTick_Difficulty() { return SpawnTickDifficultyCurve ? SpawnTickDifficultyCurve->GetFloatValue(CurrentDifficulty) : 1; }
};
