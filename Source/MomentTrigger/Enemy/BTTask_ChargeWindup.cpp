// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_ChargeWindup.h"
#include "AIController.h"
#include "Kismet/GameplayStatics.h"
#include "Enemy/EnemyCharacter.h"


UBTTask_ChargeWindup::UBTTask_ChargeWindup()
{
	NodeName = TEXT("ChargeWindup");
	INIT_TASK_NODE_NOTIFY_FLAGS();
	
}

EBTNodeResult::Type UBTTask_ChargeWindup::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	
	AEnemyCharacter* AICharacter = Cast<AEnemyCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (!AICharacter)
	{
	return EBTNodeResult::Failed;
	}
	if (AICharacter->ChargeCooldownRemaining > 0.0f)
	{
		return EBTNodeResult::Failed;
	}
		ElapsedTime = 0.0f;
		return EBTNodeResult::InProgress;
}

void UBTTask_ChargeWindup::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	
	ElapsedTime += DeltaSeconds;
	AEnemyCharacter* EnemyCharacter = Cast<AEnemyCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (!PlayerPawn || !EnemyCharacter)
	{
		return;
	}
	EnemyCharacter->ChargeDirection = (PlayerPawn->GetActorLocation() - EnemyCharacter->GetActorLocation()).GetSafeNormal();
	if (ElapsedTime >= EnemyCharacter->ChargeWindupDuration)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
