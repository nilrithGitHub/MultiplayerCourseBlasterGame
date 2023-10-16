// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseAICharacter.h"
#include "Blaster/Blaster.h"
#include "Blaster/GameState/BlasterGameState.h"
#include "Components/BoxComponent.h"

ABaseAICharacter::ABaseAICharacter()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ABaseAICharacter::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		OnTakeAnyDamage.AddDynamic(this, &ABaseAICharacter::ReceiveDamage);
	}
}

void ABaseAICharacter::ReceiveDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatorController, AActor* DamageCauser)
{
	Super::ReceiveDamage(DamagedActor, Damage, DamageType, InstigatorController, DamageCauser);

	if (Health == 0.f)
	{
		if (BlasterGameMode)
		{
			BlasterPlayerController = BlasterPlayerController == nullptr ? Cast<ABlasterPlayerController>(Controller) : BlasterPlayerController;
			ABlasterPlayerController* AttackerController = Cast<ABlasterPlayerController>(InstigatorController);
			BlasterGameMode->PlayerEliminated(this, BlasterPlayerController, AttackerController);
		}
	}
}
