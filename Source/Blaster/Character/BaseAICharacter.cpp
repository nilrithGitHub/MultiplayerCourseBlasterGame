// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseAICharacter.h"
#include "Blaster/Blaster.h"
#include "Components/BoxComponent.h"

ABaseAICharacter::ABaseAICharacter()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ABaseAICharacter::BeginPlay()
{
	Super::BeginPlay();
	//if (HasAuthority())
	//{
		OnTakeAnyDamage.AddDynamic(this, &ABaseAICharacter::ReceiveDamage);
	//}
}
