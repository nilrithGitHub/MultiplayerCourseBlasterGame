// Fill out your copyright notice in the Description page of Project Settings.


#include "ArrowProjectile.h"
#include "Kismet/GameplayStatics.h"
#include "Blaster/Character/BlasterCharacter.h"
#include "Blaster/PlayerController/BlasterPlayerController.h"
#include "Blaster/BlasterComponents/LagCompensationComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "NiagaraComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/BoxComponent.h"

AArrowProjectile::AArrowProjectile()
{
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	ProjectileMovementComponent->InitialSpeed = InitialSpeed;
	ProjectileMovementComponent->MaxSpeed = InitialSpeed;
	ProjectileMovementComponent->SetIsReplicated(true);
}

#if WITH_EDITOR
void AArrowProjectile::PostEditChangeProperty(FPropertyChangedEvent& Event)
{
	Super::PostEditChangeProperty(Event);

	FName PropertyName = Event.Property != nullptr ? Event.Property->GetFName() : NAME_None;
	if (PropertyName == GET_MEMBER_NAME_CHECKED(AArrowProjectile, InitialSpeed))
	{
		if (ProjectileMovementComponent)
		{
			ProjectileMovementComponent->InitialSpeed = InitialSpeed;
			ProjectileMovementComponent->MaxSpeed = InitialSpeed;
		}
	}
}
#endif

//void AArrowProjectile::SetInitialSpeed(float NewSpeed)
//{
//	Super::SetInitialSpeed(NewSpeed);
//	ProjectileMovementComponent->InitialSpeed = NewSpeed;
//	ProjectileMovementComponent->MaxSpeed = NewSpeed;
//	ProjectileMovementComponent->Velocity = ProjectileMovementComponent->Velocity.GetSafeNormal() * NewSpeed;
//}

void AArrowProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	ABlasterCharacter* OwnerCharacter = Cast<ABlasterCharacter>(GetOwner());
	if (OwnerCharacter)
	{
		ABlasterPlayerController* OwnerController = Cast<ABlasterPlayerController>(OwnerCharacter->Controller);
		if (OwnerController)
		{
			if (OwnerCharacter->HasAuthority() && !bUseServerSideRewind)
			{

				const float DamageToCause = Hit.BoneName.ToString() == FString("head") ? HeadShotDamage : Damage;

				UGameplayStatics::ApplyDamage(OtherActor, DamageToCause, OwnerController, this, UDamageType::StaticClass());
				Super::OnHit(HitComp, OtherActor, OtherComp, NormalImpulse, Hit);
				/*if (trailsystemcomponent)
				{
					trailsystemcomponent->destroycomponent();
				}
				if (tracercomponent)
				{
					tracercomponent->destroycomponent();
				}
				collisionbox->setcollisionenabled(ecollisionenabled::nocollision);
				projectilemovementcomponent->stopmovementimmediately();*/
				return;
			}
			ABlasterCharacter* HitCharacter = Cast<ABlasterCharacter>(OtherActor);
			if (bUseServerSideRewind && OwnerCharacter->GetLagCompensation() && OwnerCharacter->IsLocallyControlled() && HitCharacter)
			{
				OwnerCharacter->GetLagCompensation()->ProjectileServerScoreRequest(
					HitCharacter,
					TraceStart,
					InitialVelocity,
					OwnerController->GetServerTime() - OwnerController->SingleTripTime
				);
			}
		}
	}

	Super::OnHit(HitComp, OtherActor, OtherComp, NormalImpulse, Hit);
	/*if (TracerComponent)
	{
		TracerComponent->DestroyComponent();
	}
	if (TrailSystemComponent)
	{
		TrailSystemComponent->DestroyComponent();
	}
	CollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ProjectileMovementComponent->StopMovementImmediately();*/
}

void AArrowProjectile::BeginPlay()
{
	Super::BeginPlay();
	/*if (ProjectileMovementComponent)
	{
		ProjectileMovementComponent->Velocity = ProjectileMovementComponent->Velocity.GetSafeNormal() * InitialSpeed * ChargePower;
		FString TheFloatStr = FString::SanitizeFloat(ProjectileMovementComponent->Velocity.Length());
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, *TheFloatStr);
	}*/
	/*ProjectileMovementComponent->InitialSpeed = InitialSpeed;
	ProjectileMovementComponent->MaxSpeed = InitialSpeed;*/
	/*
	FPredictProjectilePathParams PathParams;
	PathParams.bTraceWithChannel = true;
	PathParams.bTraceWithCollision = true;
	PathParams.DrawDebugTime = 5.f;
	PathParams.DrawDebugType = EDrawDebugTrace::ForDuration;
	PathParams.LaunchVelocity = GetActorForwardVector() * InitialSpeed;
	PathParams.MaxSimTime = 4.f;
	PathParams.ProjectileRadius = 5.f;
	PathParams.SimFrequency = 30.f;
	PathParams.StartLocation = GetActorLocation();
	PathParams.TraceChannel = ECollisionChannel::ECC_Visibility;
	PathParams.ActorsToIgnore.Add(this);

	FPredictProjectilePathResult PathResult;

	UGameplayStatics::PredictProjectilePath(this, PathParams, PathResult);*/
}

void AArrowProjectile::Destroyed()
{
	if (FakeArrowModel)
	{
		UWorld* World = GetWorld();
		FActorSpawnParameters SpawnParams;
		AActor* SpawnedModel = World->SpawnActor<AActor>(FakeArrowModel, GetActorLocation(), GetActorRotation(), SpawnParams);
	}
	Super::Destroyed();
}

void AArrowProjectile::ServerSetChargePower_Implementation(float NewChargePower)
{
	MulticastSetChargePower(NewChargePower);
}

void AArrowProjectile::MulticastSetChargePower_Implementation(float NewChargePower)
{
	SetChargePower(NewChargePower);
}

void AArrowProjectile::SetChargePower(float NewChargePower)
{
	ChargePower = NewChargePower;
	if (ProjectileMovementComponent)
	{
		ProjectileMovementComponent->InitialSpeed = InitialSpeed * NewChargePower;
		ProjectileMovementComponent->MaxSpeed = InitialSpeed * NewChargePower;
		ProjectileMovementComponent->Velocity = ProjectileMovementComponent->Velocity.GetSafeNormal() * InitialSpeed * NewChargePower;

		//Damage = Damage * NewChargePower / 10.0f;

		/*FString TheFloatStr = FString::SanitizeFloat(Damage);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, *TheFloatStr);*/
	}
}
