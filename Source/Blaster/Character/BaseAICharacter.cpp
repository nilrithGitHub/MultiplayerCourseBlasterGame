// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseAICharacter.h"
#include "Blaster/Blaster.h"
#include "Blaster/GameMode/BlasterGameMode.h"
#include "Components/BoxComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

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
	
}
