// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WorldCharacterOverlay.generated.h"

/**
 * 
 */
UCLASS()
class BLASTER_API UWorldCharacterOverlay : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(meta = (BindWidget))
		class UProgressBar* HealthBar;

	UPROPERTY(meta = (BindWidget))
		UProgressBar* ShieldBar;
};
