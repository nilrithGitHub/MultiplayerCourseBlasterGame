// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Projectile.h"
#include "ArrowProjectile.generated.h"

/**
 * 
 */
UCLASS()
class BLASTER_API AArrowProjectile : public AProjectile
{
	GENERATED_BODY()
	
public:
	AArrowProjectile();

#if WITH_EDITOR
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& Event) override;
	//virtual void SetInitialSpeed(float NewSpeed) override;
#endif
	float ChargePower = 1.0f;
protected:
	UPROPERTY(EditAnywhere)
	TSubclassOf<class AActor> FakeArrowModel;

	virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) override;
	virtual void BeginPlay() override;
	virtual void Destroyed() override;
public:
	UFUNCTION(Server, Reliable)
	void ServerSetChargePower(float NewChargePower);
	UFUNCTION(NetMulticast, Reliable)
	void MulticastSetChargePower(float NewChargePower);
	void SetChargePower(float NewChargePower);
};
