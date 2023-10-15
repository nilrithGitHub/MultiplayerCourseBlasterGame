// Fill out your copyright notice in the Description page of Project Settings.


#include "BlasterCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/WidgetComponent.h"
#include "Net/UnrealNetwork.h"
//#include "Blaster/Weapon/Weapon.h"
//#include "Blaster/BlasterComponents/CombatComponent.h"
#include "Blaster/BlasterComponents/BuffComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "BlasterAnimInstance.h"
#include "Blaster/Blaster.h"
#include "Blaster/HUD/WorldCharacterOverlay.h"
//#include "Blaster/PlayerController/BlasterPlayerController.h"
//#include "Blaster/GameMode/BlasterGameMode.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Particles/ParticleSystemComponent.h"
//#include "Blaster/PlayerState/BlasterPlayerState.h"
//#include "Blaster/Weapon/WeaponTypes.h"
//#include "Components/BoxComponent.h"
//#include "Blaster/BlasterComponents/LagCompensationComponent.h"
//#include "NiagaraComponent.h"
//#include "NiagaraFunctionLibrary.h"
//#include "Blaster/GameState/BlasterGameState.h"
//#include "Blaster/PlayerStart/TeamPlayerStart.h"
#include "Components/ProgressBar.h"

ABlasterCharacter::ABlasterCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	Buff = CreateDefaultSubobject<UBuffComponent>(TEXT("BuffComponent"));
	Buff->SetIsReplicated(true);

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	OverheadWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("OverheadWidget"));
	OverheadWidget->SetupAttachment(RootComponent);
	
	WorldWidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("WorldWidget"));
	WorldWidgetComp->SetupAttachment(RootComponent);
	WorldWidgetComp->SetVisibility(false);

	// Move to BasePlayerCharacter
	/*Combat = CreateDefaultSubobject<UCombatComponent>(TEXT("Combat"));
	Combat->SetIsReplicated(true);

	// Move to BasePlayerCharacter
	LagCompensation = CreateDefaultSubobject<ULagCompensationComponent>(TEXT("LagCompensation"));*/

	GetCharacterMovement()->NavAgentProps.bCanCrouch = true;
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetMesh()->SetCollisionObjectType(ECC_SkeletalMesh);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetCharacterMovement()->RotationRate = FRotator(0.f, 0.f, 850.f);

	TurningInPlace = ETurningInPlace::ETIP_NotTurning;
	NetUpdateFrequency = 66.f;
	MinNetUpdateFrequency = 33.f;

	DissolveTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("DissolveTimelineComponent"));

	// Move to BasePlayerCharacter
	/*AttachedGrenade = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Attached Grenade"));
	AttachedGrenade->SetupAttachment(GetMesh(), FName("GrenadeSocket"));
	AttachedGrenade->SetCollisionEnabled(ECollisionEnabled::NoCollision);*/

	/**
	* Hit boxes for server-side rewind
	*/

	//head = CreateDefaultSubobject<UBoxComponent>(TEXT("head"));
	//head->SetupAttachment(GetMesh(), HeadName);
	//HitCollisionBoxes.Add(HeadName, head);

	//pelvis = CreateDefaultSubobject<UBoxComponent>(TEXT("pelvis"));
	//pelvis->SetupAttachment(GetMesh(), PelvisName);
	//HitCollisionBoxes.Add(PelvisName, pelvis);

	/*spine_02 = CreateDefaultSubobject<UBoxComponent>(TEXT("spine_02"));
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

	blanket = CreateDefaultSubobject<UBoxComponent>(TEXT("blanket"));
	blanket->SetupAttachment(GetMesh(), BlanketName);
	HitCollisionBoxes.Add(BlanketName, blanket);

	backpack = CreateDefaultSubobject<UBoxComponent>(TEXT("backpack"));
	backpack->SetupAttachment(GetMesh(), BackpackName);
	HitCollisionBoxes.Add(BackpackName, backpack);

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
	HitCollisionBoxes.Add(FootRName, foot_r);*/

	/*for (auto Box : HitCollisionBoxes)
	{
		if (Box.Value)
		{
			Box.Value->SetCollisionObjectType(ECC_HitBox);
			Box.Value->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
			Box.Value->SetCollisionResponseToChannel(ECC_HitBox, ECollisionResponse::ECR_Block);
			Box.Value->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
	}*/
}

void ABlasterCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Move to BasePlayerCharacter
	//DOREPLIFETIME_CONDITION(ABlasterCharacter, OverlappingWeapon, COND_OwnerOnly);
	DOREPLIFETIME(ABlasterCharacter, Health);
	DOREPLIFETIME(ABlasterCharacter, Shield);
	DOREPLIFETIME(ABlasterCharacter, bDisableGameplay);
	DOREPLIFETIME_CONDITION(ABlasterCharacter, bIsVisibleWorldWidget, COND_SkipOwner);
}

void ABlasterCharacter::OnRep_ReplicatedMovement()
{
	Super::OnRep_ReplicatedMovement();
	SimProxiesTurn();
	TimeSinceLastMovementReplication = 0.f;
}

void ABlasterCharacter::Elim(bool bPlayerLeftGame)
{
	// Move to BasePlayerCharacter
	/*DropOrDestroyWeapons();*/
	MulticastElim(bPlayerLeftGame);
}

void ABlasterCharacter::MulticastElim_Implementation(bool bPlayerLeftGame)
{
	bLeftGame = bPlayerLeftGame;
	// Move to BasePlayerCharacter
	/*if (BlasterPlayerController)
	{
		BlasterPlayerController->SetHUDWeaponAmmo(0);
	}*/
	bElimmed = true;
	PlayElimMontage();
	// Start dissolve effect
	if (DissolveMaterialInstance)
	{
		DynamicDissolveMaterialInstance = UMaterialInstanceDynamic::Create(DissolveMaterialInstance, this);
		GetMesh()->SetMaterial(0, DynamicDissolveMaterialInstance);
		DynamicDissolveMaterialInstance->SetScalarParameterValue(TEXT("Dissolve"), 0.55f);
		DynamicDissolveMaterialInstance->SetScalarParameterValue(TEXT("Glow"), 200.f);
	}
	StartDissolve();

	// Disable character movement
	bDisableGameplay = true;
	GetCharacterMovement()->DisableMovement();

	// Move to BasePlayerCharacter
	//if (Combat)
	//{
	//	Combat->FireButtonPressed(false);
	//}
	// Disable collision
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// Move to BasePlayerCharacter
	/*AttachedGrenade->SetCollisionEnabled(ECollisionEnabled::NoCollision);*/

	// Spawn elim bot
	if (ElimBotEffect)
	{
		FVector ElimBotSpawnPoint(GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z + 200.f);
		ElimBotComponent = UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			ElimBotEffect,
			ElimBotSpawnPoint,
			GetActorRotation()
		);
	}
	if (ElimBotSound)
	{
		UGameplayStatics::SpawnSoundAtLocation(
			this,
			ElimBotSound,
			GetActorLocation()
		);
	}

	// Move to BasePlayerCharacter
	/*bool bHideSniperScope = IsLocallyControlled() &&
		Combat &&
		Combat->bAiming &&
		Combat->EquippedWeapon &&
		Combat->EquippedWeapon->GetWeaponType() == EWeaponType::EWT_SniperRifle;*/

	// Move to BasePlayerCharacter

	/*if (bHideSniperScope)
	{
		ShowSniperScopeWidget(false);
	}

	// Move to BasePlayerCharacter
	if (CrownComponent)
	{
		CrownComponent->DestroyComponent();
	}*/

	GetWorldTimerManager().SetTimer(
		ElimTimer,
		this,
		&ABlasterCharacter::ElimTimerFinished,
		ElimDelay
	);
}

void ABlasterCharacter::ElimTimerFinished()
{
	if (bLeftGame && IsLocallyControlled())
	{
		OnLeftGame.Broadcast();
	}
}

// Move To BasePlayerCharacter
//void ABlasterCharacter::ServerLeaveGame_Implementation()
//{
//	BlasterGameMode = BlasterGameMode == nullptr ? GetWorld()->GetAuthGameMode<ABlasterGameMode>() : BlasterGameMode;
//	BlasterPlayerState = BlasterPlayerState == nullptr ? GetPlayerState<ABlasterPlayerState>() : BlasterPlayerState;
//	if (BlasterGameMode && BlasterPlayerState)
//	{
//		BlasterGameMode->PlayerLeftGame(BlasterPlayerState);
//	}
//}

// Move to BasePlayerCharacter
//void ABlasterCharacter::DropOrDestroyWeapon(AWeapon* Weapon)
//{
// Move to BasePlayerCharacter
//	if (Weapon == nullptr) return;
//	if (Weapon->bDestroyWeapon)
//	{
//		Weapon->Destroy();
//	}
//	else
//	{
//		Weapon->Dropped();
//	}
//}

// Move to BasePlayerCharacter
//void ABlasterCharacter::DropOrDestroyWeapons()
//{
//	if (Combat)
//	{
// Move to BasePlayerCharacter
//		if (Combat->EquippedWeapon)
//		{
//			DropOrDestroyWeapon(Combat->EquippedWeapon);
//		}
// Move to BasePlayerCharacter
//		if (Combat->SecondaryWeapon)
//		{
//			DropOrDestroyWeapon(Combat->SecondaryWeapon);
//		}
// Move to BasePlayerCharacter
//		if (Combat->TheFlag)
//		{
//			Combat->TheFlag->Dropped();
//		}
//	}
//}

// Move to BasePlayerCharacter
//void ABlasterCharacter::OnPlayerStateInitialized()
//{
//	BlasterPlayerState->AddToScore(0.f);
//	BlasterPlayerState->AddToDefeats(0);
//	SetTeamColor(BlasterPlayerState->GetTeam());
//	SetSpawnPoint();
//}

void ABlasterCharacter::SetSpawnPoint()
{
	// Move to BasePlayerCharacter
	/*if (HasAuthority() && BlasterPlayerState->GetTeam() != ETeam::ET_NoTeam)
	{
		TArray<AActor*> PlayerStarts;
		UGameplayStatics::GetAllActorsOfClass(this, ATeamPlayerStart::StaticClass(), PlayerStarts);
		TArray<ATeamPlayerStart*> TeamPlayerStarts;
		for (auto Start : PlayerStarts)
		{
			ATeamPlayerStart* TeamStart = Cast<ATeamPlayerStart>(Start);
			if (TeamStart && TeamStart->Team == BlasterPlayerState->GetTeam())
			{
				TeamPlayerStarts.Add(TeamStart);
			}
		}
		if (TeamPlayerStarts.Num() > 0)
		{
			ATeamPlayerStart* ChosenPlayerStart = TeamPlayerStarts[FMath::RandRange(0, TeamPlayerStarts.Num() - 1)];
			SetActorLocationAndRotation(
				ChosenPlayerStart->GetActorLocation(),
				ChosenPlayerStart->GetActorRotation()
			);
		}
	}*/
}

void ABlasterCharacter::Destroyed()
{
	Super::Destroyed();

	if (ElimBotComponent)
	{
		ElimBotComponent->DestroyComponent();
	}

	// Move to BasePlayerCharacter
	/*BlasterGameMode = BlasterGameMode == nullptr ? GetWorld()->GetAuthGameMode<ABlasterGameMode>() : BlasterGameMode;
	bool bMatchNotInProgress = BlasterGameMode && BlasterGameMode->GetMatchState() != MatchState::InProgress;
	if (Combat && Combat->EquippedWeapon && bMatchNotInProgress)
	{
		Combat->EquippedWeapon->Destroy();
	}*/
}

// Move to BasePlayerCharacter
//void ABlasterCharacter::MulticastGainedTheLead_Implementation()
//{
//	if (CrownSystem == nullptr) return;
//	if (CrownComponent == nullptr)
//	{
//		CrownComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(
//			CrownSystem,
//			GetMesh(),
//			FName(),
//			GetActorLocation() + FVector(0.f, 0.f, 110.f),
//			GetActorRotation(),
//			EAttachLocation::KeepWorldPosition,
//			false
//		);
//	}
// Move to BasePlayerCharacter
//	if (CrownComponent)
//	{
//		CrownComponent->Activate();
//	}
//}

// Move to BasePlayerCharacter
//void ABlasterCharacter::MulticastLostTheLead_Implementation()
//{
//	if (CrownComponent)
//	{
//		CrownComponent->DestroyComponent();
//	}
//}

// Move to BasePlayerCharacter
//void ABlasterCharacter::SetTeamColor(ETeam Team)
//{
//	if (GetMesh() == nullptr || OriginalMaterial == nullptr) return;
//	switch (Team)
//	{
//	case ETeam::ET_NoTeam:
//		GetMesh()->SetMaterial(0, OriginalMaterial);
//		DissolveMaterialInstance = BlueDissolveMatInst;
//		break;
// Move to BasePlayerCharacter
//	case ETeam::ET_BlueTeam:
//		GetMesh()->SetMaterial(0, BlueMaterial);
//		DissolveMaterialInstance = BlueDissolveMatInst;
//		break;
// Move to BasePlayerCharacter
//	case ETeam::ET_RedTeam:
//		GetMesh()->SetMaterial(0, RedMaterial);
//		DissolveMaterialInstance = RedDissolveMatInst;
//		break;
//	}
//}

void ABlasterCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		OnTakeAnyDamage.AddDynamic(this, &ABlasterCharacter::ReceiveDamage);
	}
	// Move to BasePlayerCharacter
	/*if (AttachedGrenade)
	{
		AttachedGrenade->SetVisibility(false);
	}*/
}

void ABlasterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	RotateInPlace(DeltaTime);
	// Move to BasePlayerCharacter
	//HideCameraIfCharacterClose();
	PollInit();
}

void ABlasterCharacter::RotateInPlace(float DeltaTime)
{
	// Move to BasePlayerCharacter
	/*if (Combat && Combat->bHoldingTheFlag)
	{
		bUseControllerRotationYaw = false;
		GetCharacterMovement()->bOrientRotationToMovement = true;
		TurningInPlace = ETurningInPlace::ETIP_NotTurning;
		return;
	}
	// Move to BasePlayerCharacter
	if (Combat && Combat->EquippedWeapon) GetCharacterMovement()->bOrientRotationToMovement = false;
	if (Combat && Combat->EquippedWeapon) bUseControllerRotationYaw = true;*/
	/*if (bDisableGameplay)
	{
		bUseControllerRotationYaw = false;
		TurningInPlace = ETurningInPlace::ETIP_NotTurning;
		return;
	}*/
	/*if (GetLocalRole() > ENetRole::ROLE_SimulatedProxy && IsLocallyControlled())
	{
		AimOffset(DeltaTime);
	}
	else
	{
	// Move to BasePlayerCharacter
		TimeSinceLastMovementReplication += DeltaTime;
		if (TimeSinceLastMovementReplication > 0.25f)
		{
			OnRep_ReplicatedMovement();
		}
		CalculateAO_Pitch();
	}*/
}

// Move to BasePlayerCharacter
//void ABlasterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
//{
//	Super::SetupPlayerInputComponent(PlayerInputComponent);
//
//	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ABlasterCharacter::Jump);
//
//	PlayerInputComponent->BindAxis("MoveForward", this, &ABlasterCharacter::MoveForward);
//	PlayerInputComponent->BindAxis("MoveRight", this, &ABlasterCharacter::MoveRight);
//	PlayerInputComponent->BindAxis("Turn", this, &ABlasterCharacter::Turn);
//	PlayerInputComponent->BindAxis("LookUp", this, &ABlasterCharacter::LookUp);
//
//	PlayerInputComponent->BindAction("Equip", IE_Pressed, this, &ABlasterCharacter::EquipButtonPressed);
//	PlayerInputComponent->BindAction("SwitchWeapon", IE_Pressed, this, &ABlasterCharacter::SwitchButtonPressed);
//	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &ABlasterCharacter::CrouchButtonPressed);
//	PlayerInputComponent->BindAction("Aim", IE_Pressed, this, &ABlasterCharacter::AimButtonPressed);
//	PlayerInputComponent->BindAction("Aim", IE_Released, this, &ABlasterCharacter::AimButtonReleased);
//	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ABlasterCharacter::FireButtonPressed);
//	PlayerInputComponent->BindAction("Fire", IE_Released, this, &ABlasterCharacter::FireButtonReleased);
//	PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &ABlasterCharacter::ReloadButtonPressed);
//	PlayerInputComponent->BindAction("ThrowGrenade", IE_Pressed, this, &ABlasterCharacter::GrenadeButtonPressed);
//}

void ABlasterCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	// Move to BasePlayerCharacter
	//if (Combat)
	//{
	//	Combat->Character = this;
	//}
	if (Buff)
	{
		Buff->Character = this;
		Buff->SetInitialSpeeds(
			GetCharacterMovement()->MaxWalkSpeed,
			GetCharacterMovement()->MaxWalkSpeedCrouched
		);
		Buff->SetInitialJumpVelocity(GetCharacterMovement()->JumpZVelocity);
	}
	// Move to BasePlayerCharacter
	//if (LagCompensation)
	//{
	//	LagCompensation->Character = this;
	//	if (Controller)
	//	{
	//		LagCompensation->Controller = Cast<ABlasterPlayerController>(Controller);
	//	}
	//}
}

// Move to BasePlayerCharacter
//void ABlasterCharacter::PlayFireMontage(bool bAiming)
//{
//	if (Combat == nullptr || Combat->EquippedWeapon == nullptr) return;
//
//	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
//	if (AnimInstance)
//	{
//		switch (Combat->EquippedWeapon->FireType)
//		{
// Move to BasePlayerCharacter
//		case EFireType::EFT_Charge:
//			if (FireBowMontage)
//			{
//				AnimInstance->Montage_Play(FireBowMontage);
//			}
//		default:
// Move to BasePlayerCharacter
//			if (FireWeaponMontage)
//			{
//				AnimInstance->Montage_Play(FireWeaponMontage);
//				FName SectionName;
//				SectionName = bAiming ? FName("RifleAim") : FName("RifleHip");
//				AnimInstance->Montage_JumpToSection(SectionName);
//			}
//			break;
//		}
//	}
//}

// Move to BasePlayerCharacter
//void ABlasterCharacter::PlayReloadMontage()
//{
//	if (Combat == nullptr || Combat->EquippedWeapon == nullptr) return;
// 
// 
// Move to BasePlayerCharacter
//	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
//	if (AnimInstance && ReloadMontage)
//	{
// Move to BasePlayerCharacter
//		AnimInstance->Montage_Play(ReloadMontage);
//		FName SectionName;
//
//		switch (Combat->EquippedWeapon->GetWeaponType())
//		{
// Move to BasePlayerCharacter
//		case EWeaponType::EWT_AssaultRifle:
//			SectionName = FName("Rifle");
//			break;
// Move to BasePlayerCharacter
//		case EWeaponType::EWT_RocketLauncher:
//			SectionName = FName("RocketLauncher");
//			break;
// Move to BasePlayerCharacter
//		case EWeaponType::EWT_Pistol:
//			SectionName = FName("Pistol");
//			break;
// Move to BasePlayerCharacter
//		case EWeaponType::EWT_SubmachineGun:
//			SectionName = FName("Pistol");
//			break;
// Move to BasePlayerCharacter
//		case EWeaponType::EWT_Shotgun:
//			SectionName = FName("Shotgun");
//			break;
// Move to BasePlayerCharacter
//		case EWeaponType::EWT_SniperRifle:
//			SectionName = FName("SniperRifle");
//			break;
// Move to BasePlayerCharacter
//		case EWeaponType::EWT_GrenadeLauncher:
//			SectionName = FName("GrenadeLauncher");
//			break;
//		}
//
//		AnimInstance->Montage_JumpToSection(SectionName);
//	}
//}

void ABlasterCharacter::PlayElimMontage()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && ElimMontage)
	{
		AnimInstance->Montage_Play(ElimMontage);
	}
}

// Move to BasePlayerCharacter
//void ABlasterCharacter::PlayThrowGrenadeMontage()
//{
//	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
//	if (AnimInstance && ThrowGrenadeMontage)
//	{
//		AnimInstance->Montage_Play(ThrowGrenadeMontage);
//	}
//}

// Move to BasePlayerCharacter
//void ABlasterCharacter::PlaySwapMontage()
//{
//	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
//	if (AnimInstance && SwapMontage)
//	{
//		AnimInstance->Montage_Play(SwapMontage);
//	}
//}

void ABlasterCharacter::PlayHitReactMontage()
{
	// Move to BasePlayerCharacter
	//if (Combat == nullptr || Combat->EquippedWeapon == nullptr) return;

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && HitReactMontage && !AnimInstance->IsAnyMontagePlaying())
	{
		AnimInstance->Montage_Play(HitReactMontage);
		FName SectionName("FromFront");
		AnimInstance->Montage_JumpToSection(SectionName);
	}
}

// Move to BasePlayerCharacter
//void ABlasterCharacter::GrenadeButtonPressed()
//{
//	if (Combat)
//	{
//		if (Combat->bHoldingTheFlag) return;
//
//		Combat->ThrowGrenade();
//	}
//}

void ABlasterCharacter::ReceiveDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatorController, AActor* DamageCauser)
{
	// Move to BasePlayerCharacter
	/*BlasterGameMode = BlasterGameMode == nullptr ? GetWorld()->GetAuthGameMode<ABlasterGameMode>() : BlasterGameMode;
	* 
	* Move to BasePlayerCharacter
	* 
	if (bElimmed || BlasterGameMode == nullptr) return;
	Damage = BlasterGameMode->CalculateDamage(InstigatorController, Controller, Damage);*/

	float DamageToHealth = Damage;
	if (Shield > 0.f)
	{
		if (Shield >= Damage)
		{
			Shield = FMath::Clamp(Shield - Damage, 0.f, MaxShield);
			DamageToHealth = 0.f;
		}
		else
		{
			DamageToHealth = FMath::Clamp(DamageToHealth - Shield, 0.f, Damage);
			Shield = 0.f;
		}
	}

	Health = FMath::Clamp(Health - DamageToHealth, 0.f, MaxHealth);

	UpdateHUDHealth();
	UpdateHUDShield();
	PlayHitReactMontage();

	// Move to BasePlayerCharacter
	/*if (Health == 0.f)
	{
		if (BlasterGameMode)
		{
			BlasterPlayerController = BlasterPlayerController == nullptr ? Cast<ABlasterPlayerController>(Controller) : BlasterPlayerController;
			ABlasterPlayerController* AttackerController = Cast<ABlasterPlayerController>(InstigatorController);
			BlasterGameMode->PlayerEliminated(this, BlasterPlayerController, AttackerController);
		}
	}*/
}

// Move to BasePlayerCharacter
//void ABlasterCharacter::MoveForward(float Value)
//{
//	if (bDisableGameplay) return;
//	if (Controller != nullptr && Value != 0.f)
//	{
//		const FRotator YawRotation(0.f, Controller->GetControlRotation().Yaw, 0.f);
//		const FVector Direction(FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X));
//		AddMovementInput(Direction, Value);
//	}
//}

// Move to BasePlayerCharacter
//void ABlasterCharacter::MoveRight(float Value)
//{
//	if (bDisableGameplay) return;
//	if (Controller != nullptr && Value != 0.f)
//	{
//		const FRotator YawRotation(0.f, Controller->GetControlRotation().Yaw, 0.f);
//		const FVector Direction(FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y));
//		AddMovementInput(Direction, Value);
//	}
//}

// Move to BasePlayerCharacter
//void ABlasterCharacter::Turn(float Value)
//{
//	AddControllerYawInput(IsAiming() ? Value * 0.8f : Value);
//}
//
//void ABlasterCharacter::LookUp(float Value)
//{
//	AddControllerPitchInput(IsAiming() ? Value * 0.8f : Value);
//}

// Move to BasePlayerCharacter
//void ABlasterCharacter::EquipButtonPressed()
//{
//	if (bDisableGameplay) return;
//	if (Combat)
//	{
//		if (Combat->bHoldingTheFlag) return;
//		if (Combat->CombatState == ECombatState::ECS_Unoccupied) ServerEquipButtonPressed();
//	}
//}

// Move to BasePlayerCharacter
//void ABlasterCharacter::SwitchButtonPressed()
//{
//	if (bDisableGameplay) return;
//	if (Combat)
//	{
//		if (Combat->bHoldingTheFlag) return;
//		if (Combat->CombatState == ECombatState::ECS_Unoccupied) ServerSwitchButtonPressed();
//		bool bSwap = Combat->ShouldSwapWeapons() &&
//			!HasAuthority() &&
//			Combat->CombatState == ECombatState::ECS_Unoccupied &&
//			OverlappingWeapon == nullptr;
//
//		if (bSwap)
//		{
//			PlaySwapMontage();
//			Combat->CombatState = ECombatState::ECS_SwappingWeapons;
//			bFinishedSwapping = false;
//		}
//	}
//}

// Move to BasePlayerCharacter
//void ABlasterCharacter::ServerEquipButtonPressed_Implementation()
//{
//	if (Combat)
//	{
//		if (OverlappingWeapon)
//		{
//			Combat->EquipWeapon(OverlappingWeapon);
//		}
//		/*else if (Combat->ShouldSwapWeapons())
//		{
//			Combat->SwapWeapons();
//		}*/
//	}
//}

// Move to BasePlayerCharacter
//void ABlasterCharacter::ServerSwitchButtonPressed_Implementation()
//{
//	if (Combat)
//	{
//		/*if (OverlappingWeapon)
//		{
//			Combat->EquipWeapon(OverlappingWeapon);
//		}*/
//		if (Combat->ShouldSwapWeapons())
//		{
//			Combat->SwapWeapons();
//		}
//	}
//}

// Move to BasePlayerCharacter
//void ABlasterCharacter::CrouchButtonPressed()
//{
//	if (Combat && Combat->bHoldingTheFlag) return;
//	if (bDisableGameplay) return;
//	if (bIsCrouched)
//	{
//		UnCrouch();
//	}
//	else
//	{
//		Crouch();
//	}
//}

// Move to BasePlayerCharacter
//void ABlasterCharacter::ReloadButtonPressed()
//{
//	if (Combat && Combat->bHoldingTheFlag) return;
//	if (bDisableGameplay) return;
//	if (Combat)
//	{
//		Combat->Reload();
//	}
//}

// Move to BasePlayerCharacter
//void ABlasterCharacter::AimButtonPressed()
//{
//	if (Combat && Combat->bHoldingTheFlag) return;
//	if (bDisableGameplay) return;
//	if (Combat)
//	{
//		Combat->SetAiming(true);
//	}
//}

// Move to BasePlayerCharacter
//void ABlasterCharacter::AimButtonReleased()
//{
//	if (Combat && Combat->bHoldingTheFlag) return;
//	if (bDisableGameplay) return;
//	if (Combat)
//	{
//		Combat->SetAiming(false);
//	}
//}

float ABlasterCharacter::CalculateSpeed()
{
	FVector Velocity = GetVelocity();
	Velocity.Z = 0.f;
	return Velocity.Size();
}

// Move to BasePlayerCharacter
//void ABlasterCharacter::AimOffset(float DeltaTime)
//{
//	if (Combat && Combat->EquippedWeapon == nullptr) return;
//	float Speed = CalculateSpeed();
//	bool bIsInAir = GetCharacterMovement()->IsFalling();
//
//	if (Speed == 0.f && !bIsInAir) // standing still, not jumping
//	{
//		bRotateRootBone = true;
//		FRotator CurrentAimRotation = FRotator(0.f, GetBaseAimRotation().Yaw, 0.f);
//		FRotator DeltaAimRotation = UKismetMathLibrary::NormalizedDeltaRotator(CurrentAimRotation, StartingAimRotation);
//		AO_Yaw = DeltaAimRotation.Yaw;
//		if (TurningInPlace == ETurningInPlace::ETIP_NotTurning)
//		{
//			InterpAO_Yaw = AO_Yaw;
//		}
//		bUseControllerRotationYaw = true;
//		TurnInPlace(DeltaTime);
//	}
// Move to BasePlayerCharacter
//	if (Speed > 0.f || bIsInAir) // running, or jumping
//	{
//		bRotateRootBone = false;
//		StartingAimRotation = FRotator(0.f, GetBaseAimRotation().Yaw, 0.f);
//		AO_Yaw = 0.f;
//		bUseControllerRotationYaw = true;
//		TurningInPlace = ETurningInPlace::ETIP_NotTurning;
//	}
//
//	CalculateAO_Pitch();
//}

void ABlasterCharacter::CalculateAO_Pitch()
{
	AO_Pitch = GetBaseAimRotation().Pitch;
	if (AO_Pitch > 90.f && !IsLocallyControlled())
	{
		// map pitch from [270, 360) to [-90, 0)
		FVector2D InRange(270.f, 360.f);
		FVector2D OutRange(-90.f, 0.f);
		AO_Pitch = FMath::GetMappedRangeValueClamped(InRange, OutRange, AO_Pitch);
	}
}

void ABlasterCharacter::SimProxiesTurn()
{
	// Move to BasePlayerCharacter
	/*if (Combat == nullptr || Combat->EquippedWeapon == nullptr) return;*/
	bRotateRootBone = false;
	float Speed = CalculateSpeed();
	if (Speed > 0.f)
	{
		TurningInPlace = ETurningInPlace::ETIP_NotTurning;
		return;
	}

	ProxyRotationLastFrame = ProxyRotation;
	ProxyRotation = GetActorRotation();
	ProxyYaw = UKismetMathLibrary::NormalizedDeltaRotator(ProxyRotation, ProxyRotationLastFrame).Yaw;

	if (FMath::Abs(ProxyYaw) > TurnThreshold)
	{
		if (ProxyYaw > TurnThreshold)
		{
			TurningInPlace = ETurningInPlace::ETIP_Right;
		}
		else if (ProxyYaw < -TurnThreshold)
		{
			TurningInPlace = ETurningInPlace::ETIP_Left;
		}
		else
		{
			TurningInPlace = ETurningInPlace::ETIP_NotTurning;
		}
		return;
	}
	TurningInPlace = ETurningInPlace::ETIP_NotTurning;

}

void ABlasterCharacter::Jump()
{
	//if (Combat && Combat->bHoldingTheFlag) return;
	if (bDisableGameplay) return;
	if (bIsCrouched)
	{
		UnCrouch();
	}
	else
	{
		Super::Jump();
	}
}

// Move to BasePlayerCharacter
//void ABlasterCharacter::FireButtonPressed()
//{
//	if (Combat && Combat->bHoldingTheFlag) return;
//	if (bDisableGameplay) return;
//	if (Combat)
//	{
//		Combat->FireButtonPressed(true);
//	}
//}

// Move to BasePlayerCharacter
//void ABlasterCharacter::FireButtonReleased()
//{
//	if (Combat && Combat->bHoldingTheFlag) return;
//	if (bDisableGameplay) return;
// 
// Move to BasePlayerCharacter
// 
//	if (Combat)
//	{
//		Combat->FireButtonPressed(false);
//	}
//}

void ABlasterCharacter::TurnInPlace(float DeltaTime)
{

	// Move to BasePlayerCharacter soon

	if (AO_Yaw > 90.f)
	{
		TurningInPlace = ETurningInPlace::ETIP_Right;
	}
	else if (AO_Yaw < -90.f)
	{
		TurningInPlace = ETurningInPlace::ETIP_Left;
	}
	if (TurningInPlace != ETurningInPlace::ETIP_NotTurning)
	{
		InterpAO_Yaw = FMath::FInterpTo(InterpAO_Yaw, 0.f, DeltaTime, 4.f);
		AO_Yaw = InterpAO_Yaw;
		if (FMath::Abs(AO_Yaw) < 15.f)
		{
			TurningInPlace = ETurningInPlace::ETIP_NotTurning;
			StartingAimRotation = FRotator(0.f, GetBaseAimRotation().Yaw, 0.f);
		}
	}
}

// Move to BasePlayerCharacter
//void ABlasterCharacter::HideCameraIfCharacterClose()
//{
//	if (!IsLocallyControlled()) return;
//	if ((FollowCamera->GetComponentLocation() - GetActorLocation()).Size() < CameraThreshold)
//	{
//		GetMesh()->SetVisibility(false);
//		if (Combat && Combat->EquippedWeapon && Combat->EquippedWeapon->GetWeaponMesh())
//		{
//			Combat->EquippedWeapon->GetWeaponMesh()->bOwnerNoSee = true;
//		}
//		if (Combat && Combat->SecondaryWeapon && Combat->SecondaryWeapon->GetWeaponMesh())
//		{
//			Combat->SecondaryWeapon->GetWeaponMesh()->bOwnerNoSee = true;
//		}
//	}
// 
// Move to BasePlayerCharacter
// 
//	else
//	{
//		GetMesh()->SetVisibility(true);
//		if (Combat && Combat->EquippedWeapon && Combat->EquippedWeapon->GetWeaponMesh())
//		{
//			Combat->EquippedWeapon->GetWeaponMesh()->bOwnerNoSee = false;
//		}
//		if (Combat && Combat->SecondaryWeapon && Combat->SecondaryWeapon->GetWeaponMesh())
//		{
//			Combat->SecondaryWeapon->GetWeaponMesh()->bOwnerNoSee = false;
//		}
//	}
//}

void ABlasterCharacter::OnRep_Health(float LastHealth)
{
	UpdateHUDHealth();
	if (Health < LastHealth)
	{
		PlayHitReactMontage();
	}
}

void ABlasterCharacter::OnRep_Shield(float LastShield)
{
	UpdateHUDShield();
	if (Shield < LastShield)
	{
		PlayHitReactMontage();
	}
}

void ABlasterCharacter::UpdateHUDHealth()
{
	// Move to BasePlayerCharacter
	/*BlasterPlayerController = BlasterPlayerController == nullptr ? Cast<ABlasterPlayerController>(Controller) : BlasterPlayerController;
	if (BlasterPlayerController)
	{
		BlasterPlayerController->SetHUDHealth(Health, MaxHealth);
	}*/

	UWorldCharacterOverlay* WorldCharacterOverlay = Cast<UWorldCharacterOverlay>(WorldWidgetComp->GetWidget());
	if (WorldCharacterOverlay)
	{
		if (WorldCharacterOverlay->HealthBar)
		{
			const float HealthPercent = Health / MaxHealth;
			WorldCharacterOverlay->HealthBar->SetPercent(HealthPercent);
		}
	}
}

void ABlasterCharacter::UpdateHUDShield()
{
	// Move to BasePlayerCharacter
	/*BlasterPlayerController = BlasterPlayerController == nullptr ? Cast<ABlasterPlayerController>(Controller) : BlasterPlayerController;
	if (BlasterPlayerController)
	{
		BlasterPlayerController->SetHUDShield(Shield, MaxShield);
	}*/

	UWorldCharacterOverlay* WorldCharacterOverlay = Cast<UWorldCharacterOverlay>(WorldWidgetComp->GetWidget());
	if (WorldCharacterOverlay)
	{
		// Move to BasePlayerCharacter
		//BlasterHUD = BlasterHUD == nullptr ? Cast<ABlasterHUD>(GetHUD()) : BlasterHUD;
		if (WorldCharacterOverlay->ShieldBar)
		{
			const float ShieldPercent = Shield / MaxShield;
			WorldCharacterOverlay->ShieldBar->SetPercent(ShieldPercent);
			// Move to BasePlayerCharacter
			/*FString HealthText = FString::Printf(TEXT("%d/%d"), FMath::CeilToInt(Health), FMath::CeilToInt(MaxHealth));
			WorldCharacterOverlay->HealthText->SetText(FText::FromString(HealthText));*/
		}
		// Move to BasePlayerCharacter
		/*else
		{
			bInitializeHealth = true;
			HUDHealth = Health;
			HUDMaxHealth = MaxHealth;
		}*/
	}
}

// Move to BasePlayerCharacter
//void ABlasterCharacter::UpdateHUDAmmo()
//{
//	BlasterPlayerController = BlasterPlayerController == nullptr ? Cast<ABlasterPlayerController>(Controller) : BlasterPlayerController;
//	if (BlasterPlayerController && Combat && Combat->EquippedWeapon)
//	{
//		BlasterPlayerController->SetHUDCarriedAmmo(Combat->CarriedAmmo);
//		BlasterPlayerController->SetHUDWeaponAmmo(Combat->EquippedWeapon->GetAmmo());
//	}
//}

// Move to BasePlayerCharacter
//void ABlasterCharacter::SpawDefaultWeapon()
//{
//	BlasterGameMode = BlasterGameMode == nullptr ? GetWorld()->GetAuthGameMode<ABlasterGameMode>() : BlasterGameMode;
//	UWorld* World = GetWorld();
// 
// Move to BasePlayerCharacter
// 
//	if (BlasterGameMode && World && !bElimmed && DefaultWeaponClassList.Num() > 0)
//	{
//		int32 RanIndex = FMath::RandRange(0, DefaultWeaponClassList.Num() - 1);
//		AWeapon* StartingWeapon = World->SpawnActor<AWeapon>(DefaultWeaponClassList[RanIndex]);
//		StartingWeapon->bDestroyWeapon = true;
//		if (Combat)
//		{
//			Combat->EquipWeapon(StartingWeapon);
//		}
//	}
//}

void ABlasterCharacter::PollInit()
{
	// Move to BasePlayerCharacter
	/*if (BlasterPlayerState == nullptr)
	{
		BlasterPlayerState = GetPlayerState<ABlasterPlayerState>();
		if (BlasterPlayerState)
		{
			OnPlayerStateInitialized();

			ABlasterGameState* BlasterGameState = Cast<ABlasterGameState>(UGameplayStatics::GetGameState(this));

			if (BlasterGameState && BlasterGameState->TopScoringPlayers.Contains(BlasterPlayerState))
			{
				MulticastGainedTheLead();
			}
		}
	}

	// Move to BasePlayerCharacter
	if (BlasterPlayerController == nullptr)
	{
		BlasterPlayerController = BlasterPlayerController == nullptr ? Cast<ABlasterPlayerController>(Controller) : BlasterPlayerController;
		if (BlasterPlayerController)
		{
			SpawDefaultWeapon();
			UpdateHUDAmmo();
			UpdateHUDHealth();
			UpdateHUDShield();
		}
	}*/
}

void ABlasterCharacter::UpdateDissolveMaterial(float DissolveValue)
{
	if (DynamicDissolveMaterialInstance)
	{
		DynamicDissolveMaterialInstance->SetScalarParameterValue(TEXT("Dissolve"), DissolveValue);
	}
}

void ABlasterCharacter::StartDissolve()
{
	DissolveTrack.BindDynamic(this, &ABlasterCharacter::UpdateDissolveMaterial);
	if (DissolveCurve && DissolveTimeline)
	{
		DissolveTimeline->AddInterpFloat(DissolveCurve, DissolveTrack);
		DissolveTimeline->Play();
	}
}

void ABlasterCharacter::SetVisibleWorldWidget(bool bIsVisible)
{
	// Move to BasePlayerCharacter
	//bIsVisibleWorldWidget = bIsVisible;
	if (WorldWidgetComp)
	{
		WorldWidgetComp->SetVisibility(bIsVisible);
	}
}
void ABlasterCharacter::OnRep_IsVisibleWorldWidget(bool bIsVisible)
{
	if (WorldWidgetComp)
	{
		WorldWidgetComp->SetVisibility(bIsVisibleWorldWidget);
	}
}

// Move to BasePlayerCharacter
//void ABlasterCharacter::SetOverlappingWeapon(AWeapon* Weapon)
//{
//	if (OverlappingWeapon)
//	{
//		OverlappingWeapon->ShowPickupWidget(false);
//	}
//	OverlappingWeapon = Weapon;
//	if (IsLocallyControlled())
//	{
//		if (OverlappingWeapon)
//		{
//			OverlappingWeapon->ShowPickupWidget(true);
//		}
//	}
//}

// Move to BasePlayerCharacter
//void ABlasterCharacter::OnRep_OverlappingWeapon(AWeapon* LastWeapon)
//{
//	if (OverlappingWeapon)
//	{
//		OverlappingWeapon->ShowPickupWidget(true);
//	}
//	if (LastWeapon)
//	{
//		LastWeapon->ShowPickupWidget(false);
//	}
//}

// Move to BasePlayerCharacter
//bool ABlasterCharacter::IsWeaponEquipped()
//{
//	return (Combat && Combat->EquippedWeapon);
//}

// Move to BasePlayerCharacter
//bool ABlasterCharacter::IsAiming()
//{
//	return (Combat && Combat->bAiming);
//}

// Move to BasePlayerCharacter
//bool ABlasterCharacter::IsCharging()
//{
//	return (Combat && Combat->bCharging);
//}

// Move to BasePlayerCharacter
//AWeapon* ABlasterCharacter::GetEquippedWeapon()
//{
//	if (Combat == nullptr) return nullptr;
//	return Combat->EquippedWeapon;
//}

// Move to BasePlayerCharacter
//FVector ABlasterCharacter::GetHitTarget() const
//{
//	if (Combat == nullptr) return FVector();
//	return Combat->HitTarget;
//}

// Move to BasePlayerCharacter
//ECombatState ABlasterCharacter::GetCombatState() const
//{
//	if (Combat == nullptr) return ECombatState::ECS_MAX;
//	return Combat->CombatState;
//}

// Move to BasePlayerCharacter
//bool ABlasterCharacter::IsLocallyReloading()
//{
//	if (Combat == nullptr) return false;
//	return Combat->bLocallyReloading;
//}

// Move to BasePlayerCharacter
//bool ABlasterCharacter::IsHoldingTheFlag() const
//{
//	/*if (Combat == nullptr) return false;
//	return Combat->bHoldingTheFlag;*/
//	return false;
//}

// Move to BasePlayerCharacter
//ETeam ABlasterCharacter::GetTeam()
//{
//	BlasterPlayerState = BlasterPlayerState == nullptr ? GetPlayerState<ABlasterPlayerState>() : BlasterPlayerState;
//	if (BlasterPlayerState == nullptr) return ETeam::ET_NoTeam;
//	return BlasterPlayerState->GetTeam();
//}

// Move to BasePlayerCharacter
//void ABlasterCharacter::SetHoldingTheFlag(bool bHolding)
//{
//	if (Combat == nullptr) return;
//	Combat->bHoldingTheFlag = bHolding;
//}

void ABlasterCharacter::OnCrosshairesDetected_Implementation(bool bIsDetected)
{

}
