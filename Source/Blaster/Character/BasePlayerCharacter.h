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

	void PlayFireMontage(bool bAiming); // create function for playing fire montage with param b aiming type of boolean
	void PlayReloadMontage(); // create function for playing reload montage
	void PlayThrowGrenadeMontage(); // create function for playing throw grenade montage
	void PlaySwapMontage(); // create function for playing swap montage

	virtual void Elim(bool bPlayerLeftGame) override; // create virtual function for elim with param b player left game param type of boolean
	void MulticastElim_Implementation(bool bPlayerLeftGame) override; // create function for multicast elim implementation for blueprint with param b player left game type of boolean
	virtual void Destroyed() override; // create virtual function for destroyed with override

	UFUNCTION(BlueprintImplementableEvent)
		void ShowSniperScopeWidget(bool bShowScope); // create function for showing snaper scope widget with param b show scope type of boolean

	virtual void UpdateHUDHealth() override; // create virtual function for updating hud health
	virtual void UpdateHUDShield() override; // create virtual function for updating hud shield

	void UpdateHUDAmmo(); // create function for updating hud ammo

	UFUNCTION (BlueprintCallable)
	void SpawnDefaultWeapon(); // create function for spawning default weapon

	bool bFinishedSwapping = false; // create b finished swapping variable type of boolean and set to false

	UFUNCTION(Server, Reliable)
		void ServerLeaveGame(); // create function for server leave game

	UFUNCTION(NetMulticast, Reliable)
		void MulticastGainedTheLead(); // create function for multicast gained the lead

	UFUNCTION(NetMulticast, Reliable)
		void MulticastLostTheLead(); // create function for multicast loast the lead

	void SetTeamColor(ETeam Team); // create function for for setting team color with param team type of e team

protected:
	virtual void BeginPlay() override;

	void MoveForward(float Value);	// create function input for moving forward
	void MoveRight(float Value);	// create function for moving right
	void Turn(float Value);	// create function for turning
	void LookUp(float Value);	// create function for looking up
	void EquipButtonPressed();	// create function for equipping button pressed
	void SwitchButtonPressed();	// create function for switching button pressed
	void CrouchButtonPressed();	// create function for crouching button pressed
	void ReloadButtonPressed();	// create function for reloading button pressed
	void AimButtonPressed();	// create function for aiming button pressed 
	void AimButtonReleased();	// create function for aiming button released
	void AimOffset(float DeltaTime);	// create function for aiming offset by delta time value as param
	virtual void SimProxiesTurn() override;	// create virtual function for sim proxies turning

	void FireButtonPressed();	// create function for firing button pressed
	void FireButtonReleased();	// create function for firing button released
	void GrenadeButtonPressed();	// create function for grenade button pressed
	void DropOrDestroyWeapon(AWeapon* Weapon);	// create function for droping or destroying weapon by passing weapon value
	void DropOrDestroyWeapons();	// create function for droping or destroying weapons with no params
	virtual void SetSpawnPoint() override;	// create virtual function for setting spawn point
	void OnPlayerStateInitialized();	// create function for trigger when player state initialized

	virtual void ReceiveDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, class AController* InstigatorController, AActor* DamageCauser) override;
	virtual void PollInit() override;	// create virtual function for polling init
	virtual void RotateInPlace(float DeltaTime) override;	// create function for rotating in place by delta time param

private:
	UPROPERTY(EditAnywhere)
		class UBoxComponent* head;	// create head hit box variable type of u box component
	UPROPERTY(EditAnywhere, Category = "Hitbox Bone")
		FName HeadName = "head";	// create head name variable type of f name

	UPROPERTY(EditAnywhere)
		UBoxComponent* pelvis;	// create pelvis hit box variable type of u box component
	UPROPERTY(EditAnywhere, Category = "Hitbox Bone")
		FName PelvisName = "pelvis";	// create pelvis name variable type of f name

	UPROPERTY(EditAnywhere)
		UBoxComponent* spine_02;	// create spine 02 hit box variable type of u box component
	UPROPERTY(EditAnywhere, Category = "Hitbox Bone")
		FName Spine02Name = "spine_02";	// create spine 02 name variable type of f name

	UPROPERTY(EditAnywhere)
		UBoxComponent* spine_03;	// create spine 03 hit box variable type of u box component
	UPROPERTY(EditAnywhere, Category = "Hitbox Bone")
		FName Spine03Name = "spine_03";	// create spine 03 name variable type of f name

	UPROPERTY(EditAnywhere)
		UBoxComponent* upperarm_l;	// create upperarm l hit box variable type of u box component
	UPROPERTY(EditAnywhere, Category = "Hitbox Bone")
		FName UpperarmLName = "upperarm_l";	// create upper arm l name variable type of f name

	UPROPERTY(EditAnywhere)
		UBoxComponent* upperarm_r;	// create upper arm r hit box variable type of u box component
	UPROPERTY(EditAnywhere, Category = "Hitbox Bone")
		FName UpperarmRName = "upperarm_r";	// create upper arm r name variable type of f name

	UPROPERTY(EditAnywhere)
		UBoxComponent* lowerarm_l;	// create lower arm l hit box variable type of u box component
	UPROPERTY(EditAnywhere, Category = "Hitbox Bone")
		FName LowerarmLName = "lowerarm_l";	// create lower arm l name variable type of f name

	UPROPERTY(EditAnywhere)
		UBoxComponent* lowerarm_r;	// create lower arm r hit box variable type of u box component
	UPROPERTY(EditAnywhere, Category = "Hitbox Bone")
		FName LowerarmRName = "lowerarm_r";	// create lower arm r name variable type of f name

	UPROPERTY(EditAnywhere)
		UBoxComponent* hand_l;	// create hand l hit box variable type of u box component
	UPROPERTY(EditAnywhere, Category = "Hitbox Bone")
		FName HandLName = "hand_l";	// create hand l name variable type of f name

	UPROPERTY(EditAnywhere)
		UBoxComponent* hand_r;	// create hand r hit box variable type of u box component
	UPROPERTY(EditAnywhere, Category = "Hitbox Bone")
		FName HandRName = "hand_r"; // create hand r variable type of f name

	UPROPERTY(EditAnywhere)
		UBoxComponent* backpack; // create back pack hit box variable type of u box component
	UPROPERTY(EditAnywhere, Category = "Hitbox Bone")
		FName BackpackName = "backpack"; // create back pack name variable type of f name

	UPROPERTY(EditAnywhere)
		UBoxComponent* blanket;	// create blanket hit box variable type of u box component
	UPROPERTY(EditAnywhere, Category = "Hitbox Bone")
		FName BlanketName = "blanket_l"; // create blanket name variable type of f name

	UPROPERTY(EditAnywhere)
		UBoxComponent* thigh_l;	// create thigh l hit box variable type of u box component
	UPROPERTY(EditAnywhere, Category = "Hitbox Bone")
		FName ThighRName = "thigh_l";	// create thigh r name variable type of f name

	UPROPERTY(EditAnywhere)
		UBoxComponent* thigh_r;	// create thigh r hit box variable type of u box component
	UPROPERTY(EditAnywhere, Category = "Hitbox Bone")
		FName ThighLName = "thigh_r";	// create thigh l name variable type of f name

	UPROPERTY(EditAnywhere)
		UBoxComponent* calf_l;	// create calf l hit box variable type of u box compoenent
	UPROPERTY(EditAnywhere, Category = "Hitbox Bone")
		FName CalfLName = "calf_l"; // create calf l name variable type of f name

	UPROPERTY(EditAnywhere)
		UBoxComponent* calf_r;	// create calf r hit box variable type of u box component
	UPROPERTY(EditAnywhere, Category = "Hitbox Bone")
		FName CalfRName = "calf_r";	// create calf r name variable type of f name

	UPROPERTY(EditAnywhere)
		UBoxComponent* foot_l; // create foot l hit box variable type of u box component
	UPROPERTY(EditAnywhere, Category = "Hitbox Bone")
		FName FootLName = "foot_l";	// create foot l name variable type of f name

	UPROPERTY(EditAnywhere)
		UBoxComponent* foot_r;	// create foot r hit box variable type of u box component
	UPROPERTY(EditAnywhere, Category = "Hitbox Bone")
		FName FootRName = "foot_r";	// create foot r name variable type of f name

	UPROPERTY(VisibleAnywhere, Category = Camera)
		class USpringArmComponent* CameraBoom;	// create camera boom variable type of u spring arm component

	UPROPERTY(VisibleAnywhere, Category = Camera)
		class UCameraComponent* FollowCamera;	// create follow camrea variable type of u camera component

	UPROPERTY(ReplicatedUsing = OnRep_OverlappingWeapon)
		class AWeapon* OverlappingWeapon;	// create overlapping weapon variable type of a weapon

	UFUNCTION()
		void OnRep_OverlappingWeapon(AWeapon* LastWeapon);	// create function on rep overlapping weapon with last weapon param type of a weapon

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UCombatComponent* Combat;	// create combat variable type of u combat component

	/*UPROPERTY(VisibleAnywhere)
		class ULagCompensationComponent* LagCompensation;*/

	UFUNCTION(Server, Reliable)
		void ServerEquipButtonPressed(); // create function for server equip button pressed
	UFUNCTION(Server, Reliable)
		void ServerSwitchButtonPressed();	// create function for server switch button pressed

	/**
	* Animation montages
	*/

	UPROPERTY(EditAnywhere, Category = Combat)
		class UAnimMontage* FireWeaponMontage;	// create fire weapon montage variable type of u anim montage
	UPROPERTY(EditAnywhere, Category = Combat)
		UAnimMontage* FireBowMontage;	// create fire bow montage variable type of u anim montage

	UPROPERTY(EditAnywhere, Category = Combat)
		UAnimMontage* ReloadMontage; // create reload montage variable type of u anim montage

	UPROPERTY(EditAnywhere, Category = Combat)
		UAnimMontage* ThrowGrenadeMontage; // create throw grenade montage type of u anim montage

	UPROPERTY(EditAnywhere, Category = Combat)
		UAnimMontage* SwapMontage; // create swap montage variable type of u anim montage

	void HideCameraIfCharacterClose(); // create function for hiding camera if chracter close

	UPROPERTY(EditAnywhere)
		float CameraThreshold = 200.f; // create camera threshold variable type of float and set to 200 f

	UPROPERTY()
		class ABlasterPlayerController* BlasterPlayerController; // create blaster player controller

	virtual void ElimTimerFinished() override; // create virtual function for elim timer finished

	/**
	* Team colors
	*/

	UPROPERTY(EditAnywhere, Category = Elim)
		UMaterialInstance* RedDissolveMatInst; // create red dissolve mat inst variable type of u material instance

	UPROPERTY(EditAnywhere, Category = Elim)
		UMaterialInstance* RedMaterial; // create red material variable type of u material instance

	UPROPERTY(EditAnywhere, Category = Elim)
		UMaterialInstance* BlueDissolveMatInst; // create blue dissolved mat inst variable type of u material instance

	UPROPERTY(EditAnywhere, Category = Elim)
		UMaterialInstance* BlueMaterial; // create blue material variable type of u material instance

	UPROPERTY()
		class ABlasterPlayerState* BlasterPlayerState; // create blaster player state variable type of blaster player state

	UPROPERTY(EditAnywhere)
		class UNiagaraSystem* CrownSystem; // create crown system variable type of u niagara system

	UPROPERTY()
		class UNiagaraComponent* CrownComponent; // create crown component variable type of u niagara component

	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* AttachedGrenade; // create attached grenade variable type of u static mesh component

	/**
	* Default weapon
	*/

	UPROPERTY(EditAnywhere)
		TArray<TSubclassOf<AWeapon>> DefaultWeaponClassList; // create defatul weapon class list variable type of t array of t sub class of a weapon

	/*UPROPERTY()
		class ABlasterGameMode* BlasterGameMode;*/

public:
	void SetOverlappingWeapon(AWeapon* Weapon); // create function for setting overlapping weapon with weapon param
	bool IsWeaponEquipped();	// create function for checking if weapon is equipped
	bool IsAiming();	// create function for checking if aiming
	bool IsCharging();	// create function for checking if charging

	virtual AWeapon* GetEquippedWeapon() override; // create virtual function for getting equipped weapon as return type of a weapon
	FVector GetHitTarget() const;	// create const function for getting hit target return type of f vector

	FORCEINLINE UCameraComponent* GetFollowCamera() const { return FollowCamera; }	// create force in line function for getting follow camera with return type of u camera component
	ECombatState GetCombatState() const; // create function for getting combat state with return type of e combat state
	FORCEINLINE UCombatComponent* GetCombat() const { return Combat; } // create force in line function for getting combat type of u combat component
	FORCEINLINE UAnimMontage* GetReloadMontage() const { return ReloadMontage; } // create force in line function for getting reloading montage with return type of u anim montage
	FORCEINLINE UStaticMeshComponent* GetAttachedGrenade() const { return AttachedGrenade; } // create force in line function for getting attached grenade with return type of u stataic mesh component
	bool IsLocallyReloading(); // create function for checking if locally reloading or not
	//FORCEINLINE ULagCompensationComponent* GetLagCompensation() const { return LagCompensation; }
	ETeam GetTeam(); // create function for getting team with return type of e team
	void SetHoldingTheFlag(bool bHolding); // create function for setting holding the flag with b holding param type of boolean

	virtual UBoxComponent* GetHeadBox() override; // create virtual function for getting head box with return type of u box component
	virtual void JumpToShotgunEnd() override; // create virtual function for jumping to shotgun end
};
