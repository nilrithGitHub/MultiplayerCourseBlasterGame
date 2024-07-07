// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseAICharacter.h"
#include "Pret06AICharacter.generated.h"

/**
 * 
 */
UCLASS()
class BLASTER_API APret06AICharacter : public ABaseAICharacter
{
	GENERATED_BODY()
public:
	APret06AICharacter();
private:
	UPROPERTY(EditAnywhere)
	class UBoxComponent* head;
	UPROPERTY(EditAnywhere, Category = "Hitbox Bone")
	FName HeadName = "Head_M";

	UPROPERTY(EditAnywhere)
	UBoxComponent* pelvis;
	UPROPERTY(EditAnywhere, Category = "Hitbox Bone")
	FName PelvisName = "Root_M";

	UPROPERTY(EditAnywhere)
	UBoxComponent* spine_02;
	UPROPERTY(EditAnywhere, Category = "Hitbox Bone")
	FName Spine02Name = "Spine1_M";

	UPROPERTY(EditAnywhere)
	UBoxComponent* spine_03;
	UPROPERTY(EditAnywhere, Category = "Hitbox Bone")
	FName Spine03Name = "Chest_M";

	UPROPERTY(EditAnywhere)
	UBoxComponent* upperarm_l;
	UPROPERTY(EditAnywhere, Category = "Hitbox Bone")
	FName UpperarmLName = "Shoulder_L";

	UPROPERTY(EditAnywhere)
	UBoxComponent* upperarm_r;
	UPROPERTY(EditAnywhere, Category = "Hitbox Bone")
	FName UpperarmRName = "Shoulder_R";

	UPROPERTY(EditAnywhere)
	UBoxComponent* lowerarm_l;
	UPROPERTY(EditAnywhere, Category = "Hitbox Bone")
	FName LowerarmLName = "Elbow_L";

	UPROPERTY(EditAnywhere)
	UBoxComponent* lowerarm_r;
	UPROPERTY(EditAnywhere, Category = "Hitbox Bone")
	FName LowerarmRName = "Elbow_R";

	UPROPERTY(EditAnywhere)
	UBoxComponent* hand_l;
	UPROPERTY(EditAnywhere, Category = "Hitbox Bone")
	FName HandLName = "Wrist_L";

	UPROPERTY(EditAnywhere)
	UBoxComponent* hand_r;
	UPROPERTY(EditAnywhere, Category = "Hitbox Bone")
	FName HandRName = "Wrist_R";

	UPROPERTY(EditAnywhere)
	UBoxComponent* thigh_l;
	UPROPERTY(EditAnywhere, Category = "Hitbox Bone")
	FName ThighRName = "Hip_R";

	UPROPERTY(EditAnywhere)
	UBoxComponent* thigh_r;
	UPROPERTY(EditAnywhere, Category = "Hitbox Bone")
	FName ThighLName = "Hip_L";

	UPROPERTY(EditAnywhere)
	UBoxComponent* calf_l;
	UPROPERTY(EditAnywhere, Category = "Hitbox Bone")
	FName CalfLName = "Knee_L";

	UPROPERTY(EditAnywhere)
	UBoxComponent* calf_r;
	UPROPERTY(EditAnywhere, Category = "Hitbox Bone")
	FName CalfRName = "Knee_R";

	UPROPERTY(EditAnywhere)
	UBoxComponent* foot_l;
	UPROPERTY(EditAnywhere, Category = "Hitbox Bone")
	FName FootLName = "Ankle_L";

	UPROPERTY(EditAnywhere)
	UBoxComponent* foot_r;
	UPROPERTY(EditAnywhere, Category = "Hitbox Bone")
	FName FootRName = "Ankle_R";

public:
	virtual UBoxComponent* GetHeadBox() override;
	virtual void SetAILevel(int Level) override;
};
