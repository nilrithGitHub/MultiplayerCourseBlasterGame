// Fill out your copyright notice in the Description page of Project Settings.


#include "NilBlueprintFunctionLibrary.h"
#include "Json.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"
#include "Misc/FileHelper.h"
#include "Dom/JsonObject.h"
#include "Templates/SharedPointer.h"



void UNilBlueprintFunctionLibrary::SaveSettingsToJson(FSetting Setting)
{
	//FString FilePath = FPaths::ProjectDir() + "/Settings.json";

	//TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
	//JsonObject->SetNumberField(TEXT("MouseIntensity"), Setting.MouseIntensity);
	//JsonObject->SetNumberField(TEXT("MouseZoomIntensity"), Setting.MouseZoomIntensity);

	//FString OutputString;
	//TSharedRef< TJsonWriter<> > Writer = TJsonWriterFactory<>::Create(&OutputString);
	//FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);

	//FFileHelper::SaveStringToFile(OutputString, *FilePath);
}

FSetting UNilBlueprintFunctionLibrary::LoadSettingsFromJson()
{
	//FString JsonString;
	//FString FilePath = FPaths::ProjectDir() + "/Settings.json";
	FSetting Settings;
	Settings.MouseIntensity = 1.0f;
	Settings.MouseZoomIntensity = 0.85f;

	//FFileHelper::LoadFileToString(JsonString, *FilePath);

	//TSharedPtr<FJsonObject> JsonObject;
	//TSharedRef<TJsonReader<> > Reader = TJsonReaderFactory<>::Create(JsonString);
	//if (FJsonSerializer::Deserialize(Reader, JsonObject))
	//{
	//	
	//	Settings.MouseIntensity = JsonObject->GetNumberField(TEXT("MouseIntensity"));
	//	Settings.MouseZoomIntensity = JsonObject->GetNumberField(TEXT("MouseZoomIntensity"));
	//	//MyStruct.Vector = FVector::FromJsonObject(JsonObject->GetObjectField(TEXT("Vector")));
	//}

	return Settings;
}
