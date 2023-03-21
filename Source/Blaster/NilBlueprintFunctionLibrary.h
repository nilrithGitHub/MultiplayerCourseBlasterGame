// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "NilBlueprintFunctionLibrary.generated.h"

USTRUCT(BlueprintType)
struct FSetting
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
        float MouseIntensity;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
        float MouseZoomIntensity;
};

UCLASS()
class BLASTER_API UNilBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
    static void SaveSettingsToJson(FSetting Setting);
    static FSetting LoadSettingsFromJson();
};
