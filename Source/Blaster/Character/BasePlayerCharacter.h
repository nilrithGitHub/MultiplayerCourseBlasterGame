// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BlasterCharacter.h"
#include "BasePlayerCharacter.generated.h"

/**
 * 
 */
UCLASS()
class BLASTER_API ABasePlayerCharacter : public ABlasterCharacter
{
	GENERATED_BODY()
	
public:
	ABasePlayerCharacter();

	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PostInitializeComponents() override;

	/**
	* Play montages
	*/

	void PlayFireMontage(bool bAiming);
	void PlayReloadMontage();
	void PlayThrowGrenadeMontage();
	void PlaySwapMontage();

	virtual void Elim(bool bPlayerLeftGame) override;
	void MulticastElim_Implementation(bool bPlayerLeftGame) override;
	virtual void Destroyed() override;

	UFUNCTION(BlueprintImplementableEvent)
		void ShowSniperScopeWidget(bool bShowScope);

	virtual void UpdateHUDHealth() override;
	virtual void UpdateHUDShield() override;

	void UpdateHUDAmmo();

	void SpawDefaultWeapon();

	bool bFinishedSwapping = false;

	UFUNCTION(Server, Reliable)
		void ServerLeaveGame();

	UFUNCTION(NetMulticast, Reliable)
		void MulticastGainedTheLead();

	UFUNCTION(NetMulticast, Reliable)
		void MulticastLostTheLead();

	void SetTeamColor(ETeam Team);

protected:
	virtual void BeginPlay() override;

	void MoveForward(float Value);
	void MoveRight(float Value);
	void Turn(float Value);
	void LookUp(float Value);
	void EquipButtonPressed();
	void SwitchButtonPressed();
	void CrouchButtonPressed();
	void ReloadButtonPressed();
	void AimButtonPressed();
	void AimButtonReleased();
	void AimOffset(float DeltaTime);
	virtual void SimProxiesTurn() override;

	void FireButtonPressed();
	void FireButtonReleased();
	void GrenadeButtonPressed();
	void DropOrDestroyWeapon(AWeapon* Weapon);
	void DropOrDestroyWeapons();
	virtual void SetSpawnPoint() override;
	void OnPlayerStateInitialized();

	virtual void ReceiveDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, class AController* InstigatorController, AActor* DamageCauser) override;
	virtual void PollInit() override;
	virtual void RotateInPlace(float DeltaTime) override;

private:
	UPROPERTY(EditAnywhere)
		class UBoxComponent* head;
	UPROPERTY(EditAnywhere, Category = "Hitbox Bone")
		FName HeadName = "head";

	UPROPERTY(EditAnywhere)
		UBoxComponent* pelvis;
	UPROPERTY(EditAnywhere, Category = "Hitbox Bone")
		FName PelvisName = "pelvis";

	UPROPERTY(EditAnywhere)
		UBoxComponent* spine_02;
	UPROPERTY(EditAnywhere, Category = "Hitbox Bone")
		FName Spine02Name = "spine_02";

	UPROPERTY(EditAnywhere)
		UBoxComponent* spine_03;
	UPROPERTY(EditAnywhere, Category = "Hitbox Bone")
		FName Spine03Name = "spine_03";

	UPROPERTY(EditAnywhere)
		UBoxComponent* upperarm_l;
	UPROPERTY(EditAnywhere, Category = "Hitbox Bone")
		FName UpperarmLName = "upperarm_l";

	UPROPERTY(EditAnywhere)
		UBoxComponent* upperarm_r;
	UPROPERTY(EditAnywhere, Category = "Hitbox Bone")
		FName UpperarmRName = "upperarm_r";

	UPROPERTY(EditAnywhere)
		UBoxComponent* lowerarm_l;
	UPROPERTY(EditAnywhere, Category = "Hitbox Bone")
		FName LowerarmLName = "lowerarm_l";

	UPROPERTY(EditAnywhere)
		UBoxComponent* lowerarm_r;
	UPROPERTY(EditAnywhere, Category = "Hitbox Bone")
		FName LowerarmRName = "lowerarm_r";

	UPROPERTY(EditAnywhere)
		UBoxComponent* hand_l;
	UPROPERTY(EditAnywhere, Category = "Hitbox Bone")
		FName HandLName = "hand_l";

	UPROPERTY(EditAnywhere)
		UBoxComponent* hand_r;
	UPROPERTY(EditAnywhere, Category = "Hitbox Bone")
		FName HandRName = "hand_r";

	UPROPERTY(EditAnywhere)
		UBoxComponent* backpack;
	UPROPERTY(EditAnywhere, Category = "Hitbox Bone")
		FName BackpackName = "backpack";

	UPROPERTY(EditAnywhere)
		UBoxComponent* blanket;
	UPROPERTY(EditAnywhere, Category = "Hitbox Bone")
		FName BlanketName = "blanket_l";

	UPROPERTY(EditAnywhere)
		UBoxComponent* thigh_l;
	UPROPERTY(EditAnywhere, Category = "Hitbox Bone")
		FName ThighRName = "thigh_l";

	UPROPERTY(EditAnywhere)
		UBoxComponent* thigh_r;
	UPROPERTY(EditAnywhere, Category = "Hitbox Bone")
		FName ThighLName = "thigh_r";

	UPROPERTY(EditAnywhere)
		UBoxComponent* calf_l;
	UPROPERTY(EditAnywhere, Category = "Hitbox Bone")
		FName CalfLName = "calf_l";

	UPROPERTY(EditAnywhere)
		UBoxComponent* calf_r;
	UPROPERTY(EditAnywhere, Category = "Hitbox Bone")
		FName CalfRName = "calf_r";

	UPROPERTY(EditAnywhere)
		UBoxComponent* foot_l;
	UPROPERTY(EditAnywhere, Category = "Hitbox Bone")
		FName FootLName = "foot_l";

	UPROPERTY(EditAnywhere)
		UBoxComponent* foot_r;
	UPROPERTY(EditAnywhere, Category = "Hitbox Bone")
		FName FootRName = "foot_r";

	UPROPERTY(VisibleAnywhere, Category = Camera)
		class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, Category = Camera)
		class UCameraComponent* FollowCamera;

	UPROPERTY(ReplicatedUsing = OnRep_OverlappingWeapon)
		class AWeapon* OverlappingWeapon;

	UFUNCTION()
		void OnRep_OverlappingWeapon(AWeapon* LastWeapon);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UCombatComponent* Combat;

	UPROPERTY(VisibleAnywhere)
		class ULagCompensationComponent* LagCompensation;

	UFUNCTION(Server, Reliable)
		void ServerEquipButtonPressed();
	UFUNCTION(Server, Reliable)
		void ServerSwitchButtonPressed();

	/**
	* Animation montages
	*/

	UPROPERTY(EditAnywhere, Category = Combat)
		class UAnimMontage* FireWeaponMontage;
	UPROPERTY(EditAnywhere, Category = Combat)
		UAnimMontage* FireBowMontage;

	UPROPERTY(EditAnywhere, Category = Combat)
		UAnimMontage* ReloadMontage;

	UPROPERTY(EditAnywhere, Category = Combat)
		UAnimMontage* ThrowGrenadeMontage;

	UPROPERTY(EditAnywhere, Category = Combat)
		UAnimMontage* SwapMontage;

	void HideCameraIfCharacterClose();

	UPROPERTY(EditAnywhere)
		float CameraThreshold = 200.f;

	UPROPERTY()
		class ABlasterPlayerController* BlasterPlayerController;

	virtual void ElimTimerFinished() override;

	/**
	* Team colors
	*/

	UPROPERTY(EditAnywhere, Category = Elim)
		UMaterialInstance* RedDissolveMatInst;

	UPROPERTY(EditAnywhere, Category = Elim)
		UMaterialInstance* RedMaterial;

	UPROPERTY(EditAnywhere, Category = Elim)
		UMaterialInstance* BlueDissolveMatInst;

	UPROPERTY(EditAnywhere, Category = Elim)
		UMaterialInstance* BlueMaterial;

	UPROPERTY()
		class ABlasterPlayerState* BlasterPlayerState;

	UPROPERTY(EditAnywhere)
		class UNiagaraSystem* CrownSystem;

	UPROPERTY()
		class UNiagaraComponent* CrownComponent;

	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* AttachedGrenade;

	/**
	* Default weapon
	*/

	UPROPERTY(EditAnywhere)
		TArray<TSubclassOf<AWeapon>> DefaultWeaponClassList;

	UPROPERTY()
		class ABlasterGameMode* BlasterGameMode;

public:
	void SetOverlappingWeapon(AWeapon* Weapon);
	bool IsWeaponEquipped();
	bool IsAiming();
	bool IsCharging();

	AWeapon* GetEquippedWeapon();
	FVector GetHitTarget() const;

	FORCEINLINE UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	ECombatState GetCombatState() const;
	FORCEINLINE UCombatComponent* GetCombat() const { return Combat; }
	FORCEINLINE UAnimMontage* GetReloadMontage() const { return ReloadMontage; }
	FORCEINLINE UStaticMeshComponent* GetAttachedGrenade() const { return AttachedGrenade; }
	bool IsLocallyReloading();
	FORCEINLINE ULagCompensationComponent* GetLagCompensation() const { return LagCompensation; }
	ETeam GetTeam();
	void SetHoldingTheFlag(bool bHolding);
};
