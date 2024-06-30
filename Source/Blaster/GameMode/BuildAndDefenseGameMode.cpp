// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildAndDefenseGameMode.h"
#include "Blaster/GameState/BlasterGameState.h"
//#include "Blaster/Character/BasePlayerCharacter.h"
//#include "Blaster/Character/BaseAICharacter.h"
#include "Blaster/PlayerController/BlasterPlayerController.h"
#include "Blaster/PlayerState/BlasterPlayerState.h"
#include "Blaster/BuildAndDefense/EnemySpawnManager.h"
#include "Kismet/GameplayStatics.h"

ABuildAndDefenseGameMode::ABuildAndDefenseGameMode()
{
	SpawnInSec = 5.0f;
	WaveStartDelay = 5.0f;
	MaxEnemyCount = 15;
	EnemyAliveCount = 0;
}

void ABuildAndDefenseGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	ABlasterGameState* BGameState = Cast<ABlasterGameState>(UGameplayStatics::GetGameState(this));
	if (BGameState)
	{
		ABlasterPlayerState* BPState = NewPlayer->GetPlayerState<ABlasterPlayerState>();
		if (BPState && BPState->GetTeam() == ETeam::ET_NoTeam)
		{
			BGameState->BlueTeam.AddUnique(BPState);
			BPState->SetTeam(ETeam::ET_BlueTeam);
		}
	}
}

void ABuildAndDefenseGameMode::Logout(AController* Exiting)
{
	ABlasterGameState* BGameState = Cast<ABlasterGameState>(UGameplayStatics::GetGameState(this));
	ABlasterPlayerState* BPState = Exiting->GetPlayerState<ABlasterPlayerState>();
	if (BGameState && BPState)
	{
		if (BGameState->BlueTeam.Contains(BPState))
		{
			BGameState->BlueTeam.Remove(BPState);
		}
	}
}

float ABuildAndDefenseGameMode::CalculateDamage(AController* Attacker, AController* Victim, float BaseDamage)
{
	if (Attacker == nullptr || Victim == nullptr) return BaseDamage;

	ABlasterPlayerState* AttackerPState = Attacker->GetPlayerState<ABlasterPlayerState>();
	ABlasterPlayerState* VictimPState = Victim->GetPlayerState<ABlasterPlayerState>();
	if (AttackerPState == nullptr || VictimPState == nullptr) return BaseDamage;
	if (VictimPState == AttackerPState)
	{
		return BaseDamage;
	}
	if (AttackerPState->GetTeam() == VictimPState->GetTeam())
	{
		return 0.f;
	}
	return BaseDamage;
}

void ABuildAndDefenseGameMode::PlayerEliminated(ABasePlayerCharacter* ElimmedCharacter, ABlasterPlayerController* VictimController, ABlasterPlayerController* AttackerController)
{
	Super::PlayerEliminated(ElimmedCharacter, VictimController, AttackerController);
}

void ABuildAndDefenseGameMode::AIEliminated(ABaseAICharacter* ElimmedCharacter, AController* VictimController, AController* AttackerController)
{
	Super::AIEliminated(ElimmedCharacter, VictimController, AttackerController);
}

void ABuildAndDefenseGameMode::HandleMatchHasStarted()
{
	Super::HandleMatchHasStarted();

	ABlasterGameState* BGameState = Cast<ABlasterGameState>(UGameplayStatics::GetGameState(this));
	if (BGameState)
	{
		for (auto PState : BGameState->PlayerArray)
		{
			ABlasterPlayerState* BPState = Cast<ABlasterPlayerState>(PState.Get());
			if (BPState && BPState->GetTeam() == ETeam::ET_NoTeam)
			{
				BGameState->BlueTeam.AddUnique(BPState);
				BPState->SetTeam(ETeam::ET_BlueTeam);
			}
		}
	}

	StartEnemyWave();
}

void ABuildAndDefenseGameMode::SpawnRandomEnemy()
{
	if (EnemySpawnManager)
	{
		int32 Selection = FMath::RandRange(0, EnemiesWaveToSpawn.Num() - 1);
		AActor* Spawned = GetWorld()->SpawnActor<AActor>(EnemiesWaveToSpawn[Selection], EnemySpawnManager->GetRandomSpawnTransform());
	}
}

void ABuildAndDefenseGameMode::StartEnemyWave_Implementation()
{
	if (EnemiesWaveToSpawn.Num() > 0)
	{
		// Get enemy spawn manager actor from level that type of EnemySpawnManager retun as Actor
		AActor* EnemySpawnManagerActor = UGameplayStatics::GetActorOfClass(GetWorld(), AEnemySpawnManager::StaticClass()); 
		if (EnemySpawnManagerActor)
		{
			FVector SpawnLocation;
			FRotator SpawnRotator = FRotator::ZeroRotator;
			FVector SpawnScale = FVector(1, 1, 1);

			EnemySpawnManager = Cast<AEnemySpawnManager> (EnemySpawnManagerActor); // cast to AEnemySpawnManager
			if (EnemySpawnManager)
			{
				GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, this, &ABuildAndDefenseGameMode::SpawnRandomEnemy, SpawnInSec, true, WaveStartDelay);
			}
		}
	}
}
