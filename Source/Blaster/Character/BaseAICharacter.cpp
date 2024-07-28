// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseAICharacter.h"
#include "Blaster/Blaster.h"
#include "Blaster/GameMode/BlasterGameMode.h"
#include "Components/BoxComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetStringLibrary.h"

ABaseAICharacter::ABaseAICharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	GetCharacterMovement()->bUseRVOAvoidance = true;
	bUseControllerRotationYaw = true;
}

void ABaseAICharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ABaseAICharacter::ReceiveDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatorController, AActor* DamageCauser)
{
	Super::ReceiveDamage(DamagedActor, Damage, DamageType, InstigatorController, DamageCauser);

	if (bElimmed || BlasterGameMode == nullptr) return;

	if (Health == 0.f)
	{
		if (BlasterGameMode)
		{
			// BlasterPlayerController = BlasterPlayerController == nullptr ? Cast<ABlasterPlayerController>(Controller) : BlasterPlayerController;
			// ABlasterPlayerController* AttackerController = Cast<ABlasterPlayerController>(InstigatorController);
			BlasterGameMode->AIEliminated(this, Controller, InstigatorController);
		}
	}
}

void ABaseAICharacter::ElimTimerFinished()
{
	Super::ElimTimerFinished();

	BlasterGameMode = BlasterGameMode == nullptr ? GetWorld()->GetAuthGameMode<ABlasterGameMode>() : BlasterGameMode;
	if (BlasterGameMode && !bLeftGame)
	{
		BlasterGameMode->RequestAIRespawn(this, Controller);
	}
}

void ABaseAICharacter::SetAILevel(int Level)
{
	FString Row = UKismetStringLibrary::Conv_Int64ToString(Level);
	FAISpawnData Data = GetSpawnDataFromTable(Row);

	GetCharacterMovement()->MaxWalkSpeed = Data.MaxWalkSpeed;
	MaxHealth = Data.MaxHealh;
	MaxShield = Data.MaxShield;
	Health = MaxHealth;
	Shield = MaxShield;
}

void ABaseAICharacter::SetAISpawnData(FAISpawnData SpawnData)
{
	MaxHealth = SpawnData.MaxHealh;
	MaxShield = SpawnData.MaxShield;
	GetCharacterMovement()->MaxWalkSpeed = SpawnData.MaxWalkSpeed;
}

FAISpawnData ABaseAICharacter::GetSpawnDataFromTable (FString RowName)
{
	float LMaxWalkSpeed = 0;
	float LMaxHealth = 1;
	float LMaxShield = 1;

	FAISpawnData* Row = DT_AISpawn->FindRow<FAISpawnData>(FName(RowName), TEXT(""));

	if (Row)
	{
		LMaxWalkSpeed = Row->MaxWalkSpeed;
		LMaxHealth = Row->MaxHealh;
		LMaxShield = Row->MaxShield;

		FString LogMessage = FString::Printf(TEXT("MaxWalkSpeed: %f, MaxHealh: %f, MaxShield: %f"), LMaxWalkSpeed, LMaxHealth, LMaxShield);
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, (LogMessage));

		FAISpawnData data = FAISpawnData();
		data.MaxHealh = LMaxWalkSpeed;
		data.MaxHealh = LMaxHealth;
		data.MaxShield = LMaxShield;
		return data;
	}
	else
	{
		Row = DT_AISpawn->FindRow<FAISpawnData>(FName("max"), TEXT(""));
		if (Row)
		{
			LMaxWalkSpeed = Row->MaxWalkSpeed;
			LMaxHealth = Row->MaxHealh;
			LMaxShield = Row->MaxShield;

			FString LogMessage = FString::Printf(TEXT("Max Level"), LMaxWalkSpeed, LMaxHealth, LMaxShield);
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, (LogMessage));

			FAISpawnData data = FAISpawnData();
			data.MaxHealh = LMaxWalkSpeed;
			data.MaxHealh = LMaxHealth;
			data.MaxShield = LMaxShield;
			return data;
		}
	}

	return FAISpawnData();
}
