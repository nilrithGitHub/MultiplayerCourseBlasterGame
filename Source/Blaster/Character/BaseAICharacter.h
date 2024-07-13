// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BlasterCharacter.h"
#include "BaseAICharacter.generated.h"


USTRUCT (BlueprintType)
struct FAISpawnData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnData")
	float MaxWalkSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnData")
	float MaxHealh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnData")
	float MaxShield;
};


UCLASS()
class BLASTER_API ABaseAICharacter : public ABlasterCharacter
{
	GENERATED_BODY()

public:
	ABaseAICharacter();

protected:
	virtual void BeginPlay() override;
	virtual void ReceiveDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, class AController* InstigatorController, AActor* DamageCauser) override;
	virtual void ElimTimerFinished() override;
public:
	virtual void SetAILevel(int Level);
	virtual void SetAISpawnData(FAISpawnData SpawnData);
};
