// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAIControllerBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Blaster/BuildAndDefense/EnemySpawnManager.h"

void AEnemyAIControllerBase::BeginPlay()
{
	Super::BeginPlay();

	/*UBlackboardComponent* BlackboardComp = GetBlackboardComponent();
	if (BlackboardComp)
	{
		AActor* EnemySpawnManagerActor = UGameplayStatics::GetActorOfClass(GetWorld(), AEnemySpawnManager::StaticClass());
		if (EnemySpawnManagerActor)
		{
			EnemySpawnManager = Cast<AEnemySpawnManager>(EnemySpawnManagerActor);
			if (EnemySpawnManager)
			{
				FVector TargetLocation = EnemySpawnManager->GetTargetLocation();
				BlackboardComp->SetValueAsVector("TargetLocation", TargetLocation);
			}
		}
	}*/

	
}

void AEnemyAIControllerBase::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	
	//if (BlackboardComp)
	//{
	//	/*AActor* EnemySpawnManagerActor = UGameplayStatics::GetActorOfClass(GetWorld(), AEnemySpawnManager::StaticClass());
	//	if (EnemySpawnManagerActor)
	//	{
	//		EnemySpawnManager = Cast<AEnemySpawnManager>(EnemySpawnManagerActor);
	//		if (EnemySpawnManager)
	//		{
	//			FVector TargetLocation = EnemySpawnManager->GetTargetLocation();
	//			BlackboardComp->SetValueAsVector("TargetLocation", TargetLocation);
	//		}
	//	}*/


	//	TArray<AActor*> FoundActors;
	//	UGameplayStatics::GetAllActorsOfClass(this, AEnemySpawnManager::StaticClass(), FoundActors);
	//	if (FoundActors.Num() > 0)
	//	{
	//		EnemySpawnManager = Cast<AEnemySpawnManager>(FoundActors[0]);
	//		if (EnemySpawnManager)
	//		{
	//			FVector TargetLocation = EnemySpawnManager->GetTargetLocation();
	//			BlackboardComp->SetValueAsVector("TargetLocation", TargetLocation);
	//		}
	//	}
	//}

	if (BehaviorTree)
	{
		RunBehaviorTree(BehaviorTree);
	}

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(this, AEnemySpawnManager::StaticClass(), FoundActors);
	if (FoundActors.Num() > 0)
	{
		EnemySpawnManager = Cast<AEnemySpawnManager>(FoundActors[0]);
		if (EnemySpawnManager)
		{
			FVector TargetLocation = EnemySpawnManager->GetTargetLocation();
			/*UBlackboardComponent* BlackboardComp = GetBlackboardComponent();
			if (BlackboardComp)
			{
				BlackboardComp->SetValueAsVector("TargetLocation", TargetLocation);
			}*/
			GetBlackboardComponent()->SetValueAsVector("TargetLocation", TargetLocation);
		}
	}
}
