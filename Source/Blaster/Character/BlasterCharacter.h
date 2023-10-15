// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Blaster/BlasterTypes/TurningInPlace.h"
#include "Blaster/Interfaces/InteractWithCrosshairsInterface.h"
#include "Components/TimelineComponent.h"
#include "Blaster/BlasterTypes/CombatState.h"
#include "Blaster/BlasterTypes/Team.h"
#include "BlasterCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLeftGame);

UCLASS()
class BLASTER_API ABlasterCharacter : public ACharacter, public IInteractWithCrosshairsInterface
{
	GENERATED_BODY()

public:
	ABlasterCharacter();
	virtual void Tick(float DeltaTime) override;
	// Move to BasePlayerCharacter
	//virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PostInitializeComponents() override;

	/**
	* Play montages
	*/

	// Move to BasePlayerCharacter
	//void PlayFireMontage(bool bAiming);
	//void PlayReloadMontage();
	void PlayElimMontage();

	// Move to BasePlayerCharacter
	//void PlayThrowGrenadeMontage();
	//void PlaySwapMontage();

	virtual void OnRep_ReplicatedMovement() override;
	virtual void Elim(bool bPlayerLeftGame);

	UFUNCTION(NetMulticast, Reliable)
	virtual void MulticastElim (bool bPlayerLeftGame);
	virtual void Destroyed() override;

	UPROPERTY(Replicated)
	bool bDisableGameplay = false;

	// Move to BasePlayerCharacter
	//UFUNCTION(BlueprintImplementableEvent)
	//void ShowSniperScopeWidget(bool bShowScope);

	virtual void UpdateHUDHealth();
	virtual void UpdateHUDShield();

	// Move to BasePlayerCharacter
	//void UpdateHUDAmmo();
	//void SpawDefaultWeapon();

	UPROPERTY()
	TMap<FName, class UBoxComponent*> HitCollisionBoxes;


	// Move to BasePlayerCharacter
	//bool bFinishedSwapping = false;

	/*UFUNCTION(Server, Reliable)
	void ServerLeaveGame();*/

	FOnLeftGame OnLeftGame;

	// Move to BasePlayerCharacter
	//UFUNCTION(NetMulticast, Reliable)
	//void MulticastGainedTheLead();

	// Move to BasePlayerCharacter
	//UFUNCTION(NetMulticast, Reliable)
	//void MulticastLostTheLead();

	//void SetTeamColor(ETeam Team);

protected:
	virtual void BeginPlay() override;

	// Move to BasePlayerCharacter
	//void MoveForward(float Value);
	//void MoveRight(float Value);
	//void Turn(float Value);
	//void LookUp(float Value);
	//void EquipButtonPressed();
	//void SwitchButtonPressed();
	//void CrouchButtonPressed();
	//void ReloadButtonPressed();
	//void AimButtonPressed();
	//void AimButtonReleased();
	//void AimOffset(float DeltaTime);
	void CalculateAO_Pitch();
	virtual void SimProxiesTurn();
	virtual void Jump() override;
	//void FireButtonPressed();
	//void FireButtonReleased();
	void PlayHitReactMontage();
	//void GrenadeButtonPressed();
	//void DropOrDestroyWeapon(AWeapon* Weapon);
	//void DropOrDestroyWeapons();
	virtual void SetSpawnPoint();
	//void OnPlayerStateInitialized();

	UFUNCTION()
	virtual void ReceiveDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, class AController* InstigatorController, AActor* DamageCauser);
	// Poll for any relelvant classes and initialize our HUD
	virtual void PollInit();
	virtual void RotateInPlace(float DeltaTime);

	/**
	* Hit boxes used for server-side rewind
	*/

	//UPROPERTY(EditAnywhere)
	//class UBoxComponent* head;
	//UPROPERTY(EditAnywhere, Category = "Hitbox Bone")
	//	FName HeadName = "head";

	//UPROPERTY(EditAnywhere)
	//UBoxComponent* pelvis;
	//UPROPERTY(EditAnywhere, Category = "Hitbox Bone")
	//	FName PelvisName = "pelvis";

	//UPROPERTY(EditAnywhere)
	//UBoxComponent* spine_02;
	//UPROPERTY(EditAnywhere, Category = "Hitbox Bone")
	//	FName Spine02Name = "spine_02";

	//UPROPERTY(EditAnywhere)
	//UBoxComponent* spine_03;
	//UPROPERTY(EditAnywhere, Category = "Hitbox Bone")
	//	FName Spine03Name = "spine_03";

	//UPROPERTY(EditAnywhere)
	//UBoxComponent* upperarm_l;
	//UPROPERTY(EditAnywhere, Category = "Hitbox Bone")
	//	FName UpperarmLName = "upperarm_l";

	//UPROPERTY(EditAnywhere)
	//UBoxComponent* upperarm_r;
	//UPROPERTY(EditAnywhere, Category = "Hitbox Bone")
	//	FName UpperarmRName = "upperarm_r";

	//UPROPERTY(EditAnywhere)
	//UBoxComponent* lowerarm_l;
	//UPROPERTY(EditAnywhere, Category = "Hitbox Bone")
	//	FName LowerarmLName = "lowerarm_l";

	//UPROPERTY(EditAnywhere)
	//UBoxComponent* lowerarm_r;
	//UPROPERTY(EditAnywhere, Category = "Hitbox Bone")
	//	FName LowerarmRName = "lowerarm_r";

	//UPROPERTY(EditAnywhere)
	//UBoxComponent* hand_l;
	//UPROPERTY(EditAnywhere, Category = "Hitbox Bone")
	//	FName HandLName = "hand_l";

	//UPROPERTY(EditAnywhere)
	//UBoxComponent* hand_r;
	//UPROPERTY(EditAnywhere, Category = "Hitbox Bone")
	//	FName HandRName = "hand_r";

	//UPROPERTY(EditAnywhere)
	//UBoxComponent* backpack;
	//UPROPERTY(EditAnywhere, Category = "Hitbox Bone")
	//	FName BackpackName = "backpack";

	//UPROPERTY(EditAnywhere)
	//UBoxComponent* blanket;
	//UPROPERTY(EditAnywhere, Category = "Hitbox Bone")
	//	FName BlanketName = "blanket";

	//UPROPERTY(EditAnywhere)
	//UBoxComponent* thigh_l;
	//UPROPERTY(EditAnywhere, Category = "Hitbox Bone")
	//	FName ThighRName = "thigh_l";

	//UPROPERTY(EditAnywhere)
	//UBoxComponent* thigh_r;
	//UPROPERTY(EditAnywhere, Category = "Hitbox Bone")
	//	FName ThighLName = "thigh_r";

	//UPROPERTY(EditAnywhere)
	//UBoxComponent* calf_l;
	//UPROPERTY(EditAnywhere, Category = "Hitbox Bone")
	//	FName CalfLName = "calf_l";

	//UPROPERTY(EditAnywhere)
	//UBoxComponent* calf_r;
	//UPROPERTY(EditAnywhere, Category = "Hitbox Bone")
	//	FName CalfRName = "calf_r";

	//UPROPERTY(EditAnywhere)
	//UBoxComponent* foot_l;
	//UPROPERTY(EditAnywhere, Category = "Hitbox Bone")
	//	FName FootLName = "foot_l";

	//UPROPERTY(EditAnywhere)
	//UBoxComponent* foot_r;
	//UPROPERTY(EditAnywhere, Category = "Hitbox Bone")
	//	FName FootRName = "foot_r";

protected:
	
	// Move to BasePlayerCharacter
	//UPROPERTY(VisibleAnywhere, Category = Camera)
	//class USpringArmComponent* CameraBoom;

	// Move to BasePlayerCharacter
	//UPROPERTY(VisibleAnywhere, Category = Camera)
	//class UCameraComponent* FollowCamera;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UWidgetComponent* OverheadWidget;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UWidgetComponent* WorldWidgetComp;

	// Move to BasePlayerCharacter
	/*UPROPERTY(ReplicatedUsing = OnRep_OverlappingWeapon)
	class AWeapon* OverlappingWeapon;*/

	// Move to BasePlayerCharacter
	//UFUNCTION()
	//void OnRep_OverlappingWeapon(AWeapon* LastWeapon);

	/**
	* Blaster components
	*/

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	//class UCombatComponent* Combat;

	UPROPERTY(VisibleAnywhere)
	class UBuffComponent* Buff;

	// Move to BasePlayerCharacter
	//UPROPERTY(VisibleAnywhere)
	//class ULagCompensationComponent* LagCompensation;

	// Move to BasePlayerCharacter
	//UFUNCTION(Server, Reliable)
	//void ServerEquipButtonPressed();
	//UFUNCTION(Server, Reliable)
	//void ServerSwitchButtonPressed();

	float AO_Yaw;
	float InterpAO_Yaw;
	float AO_Pitch;
	FRotator StartingAimRotation;

	ETurningInPlace TurningInPlace;
	void TurnInPlace(float DeltaTime);

	/**
	* Animation montages
	*/

	/*UPROPERTY(EditAnywhere, Category = Combat)
	class UAnimMontage* FireWeaponMontage;
	UPROPERTY(EditAnywhere, Category = Combat)
		UAnimMontage* FireBowMontage;

	UPROPERTY(EditAnywhere, Category = Combat)
	UAnimMontage* ReloadMontage;*/

	UPROPERTY(EditAnywhere, Category = Combat)
	UAnimMontage* HitReactMontage;

	UPROPERTY(EditAnywhere, Category = Combat)
	UAnimMontage* ElimMontage;

	//UPROPERTY(EditAnywhere, Category = Combat)
	//UAnimMontage* ThrowGrenadeMontage;

	//UPROPERTY(EditAnywhere, Category = Combat)
	//UAnimMontage* SwapMontage;

	//void HideCameraIfCharacterClose();

	//UPROPERTY(EditAnywhere)
	//float CameraThreshold = 200.f;

	bool bRotateRootBone;
	float TurnThreshold = 0.5f;
	FRotator ProxyRotationLastFrame;
	FRotator ProxyRotation;
	float ProxyYaw;
	float TimeSinceLastMovementReplication;
	float CalculateSpeed();

	/**
	* Player health
	*/

	UPROPERTY(EditAnywhere, Category = "Stats")
	float MaxHealth = 100.f;

	UPROPERTY(ReplicatedUsing = OnRep_Health, VisibleAnywhere, Category = "Stats")
	float Health = 100.f;

	UFUNCTION()
	void OnRep_Health(float LastHealth);

	/**
	* World Widget Visisble
	*/

	UPROPERTY(ReplicatedUsing = OnRep_IsVisibleWorldWidget, VisibleAnywhere, Category = "Stats")
	bool bIsVisibleWorldWidget = false;

	UFUNCTION()
	void OnRep_IsVisibleWorldWidget(bool bIsVisible);

	/**
	* Player shield
	*/

	UPROPERTY(EditAnywhere, Category = "Stats")
	float MaxShield = 100.f;

	UPROPERTY(ReplicatedUsing = OnRep_Shield, EditAnywhere, Category = "Stats")
	float Shield = 0.f;

	UFUNCTION()
	void OnRep_Shield(float LastShield);

	//UPROPERTY()
	//class ABlasterPlayerController* BlasterPlayerController;

	bool bElimmed = false;

	FTimerHandle ElimTimer;

	UPROPERTY(EditDefaultsOnly)
	float ElimDelay = 3.f;

	virtual void ElimTimerFinished();

	bool bLeftGame = false;

	/**
	* Dissolve effect
	*/

	UPROPERTY(VisibleAnywhere)
	UTimelineComponent* DissolveTimeline;
	FOnTimelineFloat DissolveTrack;

	UPROPERTY(EditAnywhere)
	UCurveFloat* DissolveCurve;

	UFUNCTION()
	void UpdateDissolveMaterial(float DissolveValue);
	void StartDissolve();

	// Dynamic instance that we can change at runtime
	UPROPERTY(VisibleAnywhere, Category = Elim)
	UMaterialInstanceDynamic* DynamicDissolveMaterialInstance;

	// Material instance set on the Blueprint, used with the dynamic material instance
	UPROPERTY(VisibleAnywhere, Category = Elim)
	UMaterialInstance* DissolveMaterialInstance;

	/** 
	* Team colors
	*/


	// Move to BasePlayerCharacter
	//UPROPERTY(EditAnywhere, Category = Elim)
	//UMaterialInstance* RedDissolveMatInst;

	//UPROPERTY(EditAnywhere, Category = Elim)
	//UMaterialInstance* RedMaterial;

	//UPROPERTY(EditAnywhere, Category = Elim)
	//UMaterialInstance* BlueDissolveMatInst;

	//UPROPERTY(EditAnywhere, Category = Elim)
	//UMaterialInstance* BlueMaterial;

	UPROPERTY(EditAnywhere, Category = Elim)
	UMaterialInstance* OriginalMaterial;

	/**
	* Elim effects
	*/

	UPROPERTY(EditAnywhere)
	UParticleSystem* ElimBotEffect;

	UPROPERTY(VisibleAnywhere)
	UParticleSystemComponent* ElimBotComponent;

	UPROPERTY(EditAnywhere)
	class USoundCue* ElimBotSound;

	//UPROPERTY()
	//class ABlasterPlayerState* BlasterPlayerState;

	/*UPROPERTY(EditAnywhere)
	class UNiagaraSystem* CrownSystem;

	UPROPERTY()
	class UNiagaraComponent* CrownComponent;*/

	/**
	* Grenade
	*/

	//UPROPERTY(VisibleAnywhere)
	//UStaticMeshComponent* AttachedGrenade;

	/**
	* Default weapon
	*/

	//UPROPERTY(EditAnywhere)
	//TArray<TSubclassOf<AWeapon>> DefaultWeaponClassList;

	//UPROPERTY()
	//class ABlasterGameMode* BlasterGameMode;


public:
	void SetVisibleWorldWidget(bool bIsVisible);
	// Move to BasePlayerCharacter
	//void SetOverlappingWeapon(AWeapon* Weapon);
	/*bool IsWeaponEquipped();
	bool IsAiming();
	bool IsCharging();*/

	FORCEINLINE float GetAO_Yaw() const { return AO_Yaw; }
	FORCEINLINE float GetAO_Pitch() const { return AO_Pitch; }
	//AWeapon* GetEquippedWeapon();
	FORCEINLINE ETurningInPlace GetTurningInPlace() const { return TurningInPlace; }
	// Move to BasePlayerCharacter
	//FVector GetHitTarget() const;
	//FORCEINLINE UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	FORCEINLINE bool ShouldRotateRootBone() const { return bRotateRootBone; }
	FORCEINLINE bool IsElimmed() const { return bElimmed; }
	FORCEINLINE float GetHealth() const { return Health; }
	FORCEINLINE void SetHealth(float Amount) { Health = Amount; }
	FORCEINLINE float GetMaxHealth() const { return MaxHealth; }
	FORCEINLINE float GetShield() const { return Shield; }
	FORCEINLINE void SetShield(float Amount) { Shield = Amount; }
	FORCEINLINE float GetMaxShield() const { return MaxShield; }
	// Move to BasePlayerCharacter
	//ECombatState GetCombatState() const;
	//FORCEINLINE UCombatComponent* GetCombat() const { return Combat; }
	FORCEINLINE bool GetDisableGameplay() const { return bDisableGameplay; }
	// Move to BasePlayerCharacter
	//FORCEINLINE UAnimMontage* GetReloadMontage() const { return ReloadMontage; }
	//FORCEINLINE UStaticMeshComponent* GetAttachedGrenade() const { return AttachedGrenade; }
	FORCEINLINE UBuffComponent* GetBuff() const { return Buff; }
	// Move to BasePlayerCharacter
	//bool IsLocallyReloading();
	//FORCEINLINE ULagCompensationComponent* GetLagCompensation() const { return LagCompensation; }
	//FORCEINLINE bool IsHoldingTheFlag() const;
	/*ETeam GetTeam();*/
	/*void SetHoldingTheFlag(bool bHolding);*/

	// Interface
	void OnCrosshairesDetected_Implementation(bool bIsDetected);
};
