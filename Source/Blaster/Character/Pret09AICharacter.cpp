// Fill out your copyright notice in the Description page of Project Settings.


#include "Pret09AICharacter.h"
#include "Components/BoxComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Blaster/Blaster.h"

APret09AICharacter::APret09AICharacter()
{
	head = CreateDefaultSubobject<UBoxComponent>(TEXT("head"));
	head->SetupAttachment(GetMesh(), HeadName);
	HitCollisionBoxes.Add(HeadName, head);

	pelvis = CreateDefaultSubobject<UBoxComponent>(TEXT("pelvis"));
	pelvis->SetupAttachment(GetMesh(), PelvisName);
	HitCollisionBoxes.Add(PelvisName, pelvis);

	spine_02 = CreateDefaultSubobject<UBoxComponent>(TEXT("spine_02"));
	spine_02->SetupAttachment(GetMesh(), Spine02Name);
	HitCollisionBoxes.Add(Spine02Name, spine_02);

	spine_03 = CreateDefaultSubobject<UBoxComponent>(TEXT("spine_03"));
	spine_03->SetupAttachment(GetMesh(), Spine03Name);
	HitCollisionBoxes.Add(Spine03Name, spine_03);

	upperarm_l = CreateDefaultSubobject<UBoxComponent>(TEXT("upperarm_l"));
	upperarm_l->SetupAttachment(GetMesh(), UpperarmLName);
	HitCollisionBoxes.Add(UpperarmLName, upperarm_l);

	upperarm_r = CreateDefaultSubobject<UBoxComponent>(TEXT("upperarm_r"));
	upperarm_r->SetupAttachment(GetMesh(), UpperarmRName);
	HitCollisionBoxes.Add(UpperarmRName, upperarm_r);

	lowerarm_l = CreateDefaultSubobject<UBoxComponent>(TEXT("lowerarm_l"));
	lowerarm_l->SetupAttachment(GetMesh(), LowerarmLName);
	HitCollisionBoxes.Add(LowerarmLName, lowerarm_l);

	lowerarm_r = CreateDefaultSubobject<UBoxComponent>(TEXT("lowerarm_r"));
	lowerarm_r->SetupAttachment(GetMesh(), LowerarmRName);
	HitCollisionBoxes.Add(LowerarmRName, lowerarm_r);

	hand_l = CreateDefaultSubobject<UBoxComponent>(TEXT("hand_l"));
	hand_l->SetupAttachment(GetMesh(), HandLName);
	HitCollisionBoxes.Add(HandLName, hand_l);

	hand_r = CreateDefaultSubobject<UBoxComponent>(TEXT("hand_r"));
	hand_r->SetupAttachment(GetMesh(), HandRName);
	HitCollisionBoxes.Add(HandRName, hand_r);

	thigh_l = CreateDefaultSubobject<UBoxComponent>(TEXT("thigh_l"));
	thigh_l->SetupAttachment(GetMesh(), ThighLName);
	HitCollisionBoxes.Add(ThighLName, thigh_l);

	thigh_r = CreateDefaultSubobject<UBoxComponent>(TEXT("thigh_r"));
	thigh_r->SetupAttachment(GetMesh(), ThighRName);
	HitCollisionBoxes.Add(ThighRName, thigh_r);

	calf_l = CreateDefaultSubobject<UBoxComponent>(TEXT("calf_l"));
	calf_l->SetupAttachment(GetMesh(), CalfLName);
	HitCollisionBoxes.Add(CalfLName, calf_l);

	calf_r = CreateDefaultSubobject<UBoxComponent>(TEXT("calf_r"));
	calf_r->SetupAttachment(GetMesh(), CalfRName);
	HitCollisionBoxes.Add(CalfRName, calf_r);

	foot_l = CreateDefaultSubobject<UBoxComponent>(TEXT("foot_l"));
	foot_l->SetupAttachment(GetMesh(), FootLName);
	HitCollisionBoxes.Add(FootLName, foot_l);

	foot_r = CreateDefaultSubobject<UBoxComponent>(TEXT("foot_r"));
	foot_r->SetupAttachment(GetMesh(), FootRName);
	HitCollisionBoxes.Add(FootRName, foot_r);

	for (auto Box : HitCollisionBoxes)
	{
		if (Box.Value)
		{
			Box.Value->SetCollisionObjectType(ECC_HitBox);
			Box.Value->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
			Box.Value->SetCollisionResponseToChannel(ECC_HitBox, ECollisionResponse::ECR_Block);
			Box.Value->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
	}
}

UBoxComponent* APret09AICharacter::GetHeadBox()
{
	return HitCollisionBoxes[HeadName];
}

void APret09AICharacter::SetAILevel(int Level)
{
	Super::SetAILevel(Level);

	// Max Speed = 300;
	float MaxSpeed = 300;

	switch (Level)
	{
	case 1: MaxSpeed = 100; break;
	case 2: MaxSpeed = 150; break;
	case 3: MaxSpeed = 200; break;
	case 4: MaxSpeed = 250; break;
	case 5: MaxSpeed = 300; break;
	default:
		break;
	}

	GetCharacterMovement()->MaxWalkSpeed = MaxSpeed;
}
