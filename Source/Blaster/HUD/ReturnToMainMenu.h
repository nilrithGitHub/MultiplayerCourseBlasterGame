// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ReturnToMainMenu.generated.h"

/**
 *
 */
UCLASS()
class BLASTER_API UReturnToMainMenu : public UUserWidget
{
	GENERATED_BODY()
public:
	void MenuSetup();
	void MenuTearDown();

protected:
	virtual bool Initialize() override;

	UFUNCTION()
	void OnDestroySession(bool bWasSuccessful);

	UFUNCTION()
	void OnPlayerLeftGame();

private:
	UPROPERTY(meta = (BindWidget))
	class UButton* ReturnButton;
	UPROPERTY(meta = (BindWidget))
	class USlider* Slider_MouseSen;
	UPROPERTY(meta = (BindWidget))
	USlider* Slider_MouseZoomSen;

	UFUNCTION()
	void ReturnButtonClicked();
	UFUNCTION()
	void OnSliderMouseSenChanged(float Value);
	UFUNCTION()
	void OnSliderMouseZoomSenChanged(float Value);

	UPROPERTY()
	class UMultiplayerSessionsSubsystem* MultiplayerSessionsSubsystem;

	UPROPERTY()
	class APlayerController* PlayerController;
};
