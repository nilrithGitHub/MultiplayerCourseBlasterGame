// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "MeleeWeapon.generated.h"

/**
 * 
 */
UCLASS()
class BLASTER_API AMeleeWeapon : public AWeapon
{
	GENERATED_BODY()
public:
	virtual void Fire(const FVector& HitTarget) override;
protected:

	void WeaponTraceHit(const FVector& TraceStart, const FVector& TraceEnd, const FVector& HitTarget, FHitResult& OutHit);

	UPROPERTY(EditAnywhere)
		class UParticleSystem* ImpactParticles;

	UPROPERTY(EditAnywhere)
		USoundCue* HitSound;
private:

	UPROPERTY(EditAnywhere)
		UParticleSystem* BeamParticles;

	UPROPERTY(EditAnywhere)
		UParticleSystem* MuzzleFlash;

	UPROPERTY(EditAnywhere)
		USoundCue* FireSound;
};
