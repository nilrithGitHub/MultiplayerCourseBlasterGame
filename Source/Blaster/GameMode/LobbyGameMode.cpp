// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameMode.h"
#include "GameFramework/GameStateBase.h"
#include "MultiplayerSessionsSubsystem.h"

ALobbyGameMode::ALobbyGameMode()
{
	FreeForAllPath = FString("/Game/Maps/BlasterMap?listen");
	TeamsPath = FString("/Game/Maps/Teams?listen");
	CaptureTheFlagPath = FString("/Game/Maps/CaptureTheFlag?listen");
}

//void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
//{
//	Super::PostLogin(NewPlayer);
//
//	int32 NumberOfPlayers = GameState.Get()->PlayerArray.Num();
//
//	UGameInstance* GameInstance = GetGameInstance();
//	if (GameInstance)
//	{
//		UMultiplayerSessionsSubsystem* Subsystem = GameInstance->GetSubsystem<UMultiplayerSessionsSubsystem>();
//		check(Subsystem);
//
//		if (NumberOfPlayers == Subsystem->DesiredNumPublicConnections)
//		{
//			UWorld* World = GetWorld();
//			if (World)
//			{
//				bUseSeamlessTravel = true;
//
//				FString MatchType = Subsystem->DesiredMatchType;
//				if (MatchType == "FreeForAll")
//				{
//					//World->ServerTravel(FString("/Game/Maps/BlasterMap?listen"));
//					World->ServerTravel(FreeForAllPath);
//				}
//				else if (MatchType == "Teams")
//				{
//					//World->ServerTravel(FString("/Game/Maps/Teams?listen"));
//					World->ServerTravel(TeamsPath);
//				}
//				else if (MatchType == "CaptureTheFlag")
//				{
//					//World->ServerTravel(FString("/Game/Maps/CaptureTheFlag?listen"));
//					World->ServerTravel(CaptureTheFlagPath);
//				}
//			}
//		}
//	}
//}

// For Learning
void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	int32 NumberOfPlayers = GameState.Get()->PlayerArray.Num();

	UGameInstance* GameInstance = GetGameInstance();

	if (GameInstance)
	{
		UMultiplayerSessionsSubsystem* Subsystem = GameInstance->GetSubsystem<UMultiplayerSessionsSubsystem>();
		check(Subsystem);

		if (NumberOfPlayers == Subsystem->DesiredNumPublicConnections)
		{
			UWorld* World = GetWorld();
			if (World)
			{
				bUseSeamlessTravel = true;

				FString MatchType = Subsystem->DesiredMatchType;
				if (MatchType == "FreeForAll")
				{
					//World->ServerTravel(FString("/Game/Maps/BlasterMap?listen"));
					//World->ServerTravel(FreeForAllPath);
					World->ServerTravel(FString("/Game/Maps/Nil/Nil_BlasterMap1?listen"));
				}
				else if (MatchType == "Teams")
				{
					//World->ServerTravel(FString("/Game/Maps/Teams?listen"));
					//World->ServerTravel(TeamsPath);
					World->ServerTravel(FString("/Game/Maps/Nil/Nil_BlasterMap2?listen"));
				}
				else if (MatchType == "CaptureTheFlag")
				{
					//World->ServerTravel(FString("/Game/Maps/CaptureTheFlag?listen"));
					//World->ServerTravel(CaptureTheFlagPath);
					World->ServerTravel(FString("/Game/Maps/Nil/Nil_BlasterMap3?listen"));
				}
			}
		}
	}
}