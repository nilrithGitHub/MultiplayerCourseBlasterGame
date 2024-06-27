// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BlasterGameMode.h"
#include "BuildAndDefenseGameMode.generated.h"

/**
 * 
 */
UCLASS()
class BLASTER_API ABuildAndDefenseGameMode : public ABlasterGameMode
{
	GENERATED_BODY()
public:
	ABuildAndDefenseGameMode();
private:
	FTimerHandle SpawnTimerHandle;
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn Wave")
	float WaveStartDelay;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn Wave")
	float SpawnInSec;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn Wave")
	TArray<TSubclassOf<class ABaseAICharacter>> EnemiesWaveToSpawn;
	UPROPERTY()
	class AEnemySpawnManager* EnemySpawnManager;
public:
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void Logout(AController* Exiting) override;
	virtual float CalculateDamage(AController* Attacker, AController* Victim, float BaseDamage) override;
	virtual void PlayerEliminated(class ABasePlayerCharacter* ElimmedCharacter, class ABlasterPlayerController* VictimController, ABlasterPlayerController* AttackerController) override;
	virtual void AIEliminated(class ABaseAICharacter* ElimmedCharacter, class AController* VictimController, AController* AttackerController) override;
protected:
	virtual void HandleMatchHasStarted() override;
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void StartEnemyWave();
	UFUNCTION(BlueprintCallable)
	void SpawnRandomEnemy();
};
