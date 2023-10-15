// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BlasterCharacter.h"
#include "BaseAICharacter.generated.h"

UCLASS()
class BLASTER_API ABaseAICharacter : public ABlasterCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseAICharacter();

protected:
	virtual void BeginPlay() override;


};
