// Fill out your copyright notice in the Description page of Project Settings.


#include "ElimAnnouncement.h"
#include "Components/TextBlock.h"

void UElimAnnouncement::SetElimAnnouncementText(FString AttackerName, FString VictimName, FString WeaponName)
{
	//FString ElimAnnouncementText = FString::Printf(TEXT("%s elimmed %s!"), *AttackerName, *VictimName);
	if (AttackerName == "Error404" || AttackerName == "ERROR404")
		AttackerName = "Deverloper";
	if (VictimName == "Error404" || VictimName == "ERROR404")
		VictimName = "Deverloper";
	//FString ElimAnnouncementText = FString::Printf(TEXT("%s - delete - %s!"), *AttackerName, *VictimName);
	FString ElimAnnouncementText = FString::Printf(TEXT("%s -[%s]- %s!"), *AttackerName, *WeaponName, *VictimName);
	if (AnnouncementText)
	{
		AnnouncementText->SetText(FText::FromString(ElimAnnouncementText));
	}
}