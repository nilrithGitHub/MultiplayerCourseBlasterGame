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
#include "Blaster/PlayerController/BlasterPlayerController.h"
#include "Blaster/GameMode/BlasterGameMode.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Particles/ParticleSystemComponent.h"
//#include "Blaster/PlayerState/BlasterPlayerState.h"
//#include "Blaster/Weapon/WeaponTypes.h"
//#include "Components/BoxComponent.h"
#include "Blaster/BlasterComponents/LagCompensationComponent.h"
//#include "NiagaraComponent.h"
//#include "NiagaraFunctionLibrary.h"
//#include "Blaster/GameState/BlasterGameState.h"
//#include "Blaster/PlayerStart/TeamPlayerStart.h"
#include "Components/ProgressBar.h"

bool ABlasterCharacter::IsBoneHead(FString BoneName)
{
	return BoneName == FString("Head_M") ||
		BoneName == FString("Head") ||
		BoneName == FString("head");
}

ABlasterCharacter::ABlasterCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	Buff = CreateDefaultSubobject<UBuffComponent>(TEXT("BuffComponent"));
	Buff->SetIsReplicated(true);

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	OverheadWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("OverheadWidget"));
	OverheadWidget->SetupAttachment(RootComponent);
	OverheadWidget->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	OverheadWidget->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	
	WorldWidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("WorldWidget"));
	WorldWidgetComp->SetupAttachment(RootComponent);
	WorldWidgetComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WorldWidgetComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	WorldWidgetComp->SetVisibility(false);
	WorldWidgetComp->SetIsReplicated(true);

	/*Combat = CreateDefaultSubobject<UCombatComponent>(TEXT("Combat"));
	Combat->SetIsReplicated(true);*/

	LagCompensation = CreateDefaultSubobject<ULagCompensationComponent>(TEXT("LagCompensation"));

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

	/*AttachedGrenade = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Attached Grenade"));
	AttachedGrenade->SetupAttachment(GetMesh(), FName("GrenadeSocket"));
	AttachedGrenade->SetCollisionEnabled(ECollisionEnabled::NoCollision);*/
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
	/*DropOrDestroyWeapons();*/
	MulticastElim(bPlayerLeftGame);
}

void ABlasterCharacter::MulticastElim_Implementation(bool bPlayerLeftGame)
{
	bLeftGame = bPlayerLeftGame;
	/*if (BlasterPlayerController)
	{
		BlasterPlayerController->SetHUDWeaponAmmo(0);
	}*/
	bElimmed = true;

	PlayElimMontage();
	SetVisibleWorldWidget(false);

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

	//if (Combat)
	//{
	//	Combat->FireButtonPressed(false);
	//}
	// Disable collision
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

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

	/*bool bHideSniperScope = IsLocallyControlled() &&
		Combat &&
		Combat->bAiming &&
		Combat->EquippedWeapon &&
		Combat->EquippedWeapon->GetWeaponType() == EWeaponType::EWT_SniperRifle;*/

	/*if (bHideSniperScope)
	{
		ShowSniperScopeWidget(false);
	}

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
}

void ABlasterCharacter::WidgetBarVisibleFinished()
{
	if (WorldWidgetComp)
	{
		WorldWidgetComp->SetVisibility(false);
	}
}

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

void ABlasterCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		OnTakeAnyDamage.AddDynamic(this, &ABlasterCharacter::ReceiveDamage);
	}
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

void ABlasterCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	// Move to BasePlayerCharacter
	//if (Combat)
	//{
	//	Combat->Character = this;
	//}

	Health = MaxHealth;

	if (Buff)
	{
		Buff->Character = this;
		Buff->SetInitialSpeeds(
			GetCharacterMovement()->MaxWalkSpeed,
			GetCharacterMovement()->MaxWalkSpeedCrouched
		);
		Buff->SetInitialJumpVelocity(GetCharacterMovement()->JumpZVelocity);
	}

	if (LagCompensation)
	{
		LagCompensation->Character = this;
		if (Controller)
		{
			LagCompensation->Controller = Cast<ABlasterPlayerController>(Controller);
		}
	}
}

void ABlasterCharacter::PlayElimMontage()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && ElimMontage)
	{
		AnimInstance->Montage_Play(ElimMontage);
	}
}

void ABlasterCharacter::PlayHitReactMontage()
{
	// Move to BasePlayerCharacter
	//if (Combat == nullptr || Combat->EquippedWeapon == nullptr) return;

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && HitReactMontage && !AnimInstance->IsAnyMontagePlaying())
	{
		AnimInstance->Montage_Play(HitReactMontage);
		/*FName SectionName("FromFront");
		AnimInstance->Montage_JumpToSection(SectionName);*/
	}
}

void ABlasterCharacter::ReceiveDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatorController, AActor* DamageCauser)
{
	BlasterGameMode = BlasterGameMode == nullptr ? GetWorld()->GetAuthGameMode<ABlasterGameMode>() : BlasterGameMode;
	if (bElimmed || BlasterGameMode == nullptr) return;
	Damage = BlasterGameMode->CalculateDamage(InstigatorController, Controller, Damage);

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

float ABlasterCharacter::CalculateSpeed()
{
	FVector Velocity = GetVelocity();
	Velocity.Z = 0.f;
	return Velocity.Size();
}

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

	//UWorldCharacterOverlay* WorldCharacterOverlay = Cast<UWorldCharacterOverlay>(WorldWidgetComp->GetWidget());
	WorldCharacterOverlay = WorldCharacterOverlay == nullptr ? Cast<UWorldCharacterOverlay>(WorldWidgetComp->GetWidget()) : WorldCharacterOverlay;
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

	//UWorldCharacterOverlay* WorldCharacterOverlay = Cast<UWorldCharacterOverlay>(WorldWidgetComp->GetWidget());
	WorldCharacterOverlay = WorldCharacterOverlay == nullptr ? Cast<UWorldCharacterOverlay>(WorldWidgetComp->GetWidget()) : WorldCharacterOverlay;
	if (WorldCharacterOverlay)
	{
		//BlasterHUD = BlasterHUD == nullptr ? Cast<ABlasterHUD>(GetHUD()) : BlasterHUD;
		if (WorldCharacterOverlay->ShieldBar)
		{
			const float ShieldPercent = Shield / MaxShield;
			WorldCharacterOverlay->ShieldBar->SetPercent(ShieldPercent);

			/*FString HealthText = FString::Printf(TEXT("%d/%d"), FMath::CeilToInt(Health), FMath::CeilToInt(MaxHealth));
			WorldCharacterOverlay->HealthText->SetText(FText::FromString(HealthText));*/
		}
		/*else
		{
			bInitializeHealth = true;
			HUDHealth = Health;
			HUDMaxHealth = MaxHealth;
		}*/
	}
}

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
	//bIsVisibleWorldWidget = bIsVisible;
	if (WorldWidgetComp)
	{
		WorldWidgetComp->SetVisibility(bIsVisible);
	}

	if (bIsVisible)
	{
		GetWorldTimerManager().SetTimer(
			WidgetBarVisibleTimer,
			this,
			&ABlasterCharacter::WidgetBarVisibleFinished,
			WidgetBarVisibleDelay
		);
	}
	else
	{
		GetWorldTimerManager().ClearTimer(WidgetBarVisibleTimer);
	}
}
AWeapon* ABlasterCharacter::GetEquippedWeapon()
{
	return nullptr;
}
void ABlasterCharacter::OnRep_IsVisibleWorldWidget(bool bIsVisible)
{
	if (WorldWidgetComp)
	{
		WorldWidgetComp->SetVisibility(bIsVisibleWorldWidget);
	}
}

UBoxComponent* ABlasterCharacter::GetHeadBox()
{
	return HitCollisionBoxes[FName("head")];
}

void ABlasterCharacter::JumpToShotgunEnd()
{
}

void ABlasterCharacter::OnCrosshairesDetected_Implementation(bool bIsDetected)
{

}
