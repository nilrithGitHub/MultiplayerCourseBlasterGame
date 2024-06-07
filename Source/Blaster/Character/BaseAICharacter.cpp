// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseAICharacter.h"
#include "Blaster/Blaster.h"
#include "Blaster/GameMode/BlasterGameMode.h"
#include "Components/BoxComponent.h"

ABaseAICharacter::ABaseAICharacter()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ABaseAICharacter::BeginPlay()
{
	Super::BeginPlay(); // set begin game play section on start
}

void ABaseAICharacter::ReceiveDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatorController, AActor* DamageCauser)
{
	Super::ReceiveDamage(DamagedActor, Damage, DamageType, InstigatorController, DamageCauser);

	if (bElimmed || BlasterGameMode == nullptr) return;

	if (Health == 0.f)
	{
		if (BlasterGameMode)	// check if blaster game mode is not a null pointer.
		{
			//BlasterPlayerController = BlasterPlayerController == nullptr ? Cast<ABlasterPlayerController>(Controller) : BlasterPlayerController;
			//ABlasterPlayerController* AttackerController = Cast<ABlasterPlayerController>(InstigatorController);
			BlasterGameMode->AIEliminated(this, Controller, InstigatorController);	// ai eliminated in blaster game mode with param this, controller and instigator controller.
		}
	}
}

void ABaseAICharacter::ElimTimerFinished()
{
	Super::ElimTimerFinished();

	BlasterGameMode = BlasterGameMode == nullptr ? GetWorld()->GetAuthGameMode<ABlasterGameMode>() : BlasterGameMode; // Check if blaster game mode is null or not.
	if (BlasterGameMode && !bLeftGame)	// check if blaster game mode is not null pointer and not left game already.
	{
		BlasterGameMode->RequestAIRespawn(this, Controller); // request ai respawn in blaster game mode by at param this and controller as second param
	}
	//if (bLeftGame && IsLocallyControlled())
	//{
	//	OnLeftGame.Broadcast();
	//}
}
