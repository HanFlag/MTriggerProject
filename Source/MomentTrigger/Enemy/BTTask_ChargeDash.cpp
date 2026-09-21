// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_ChargeDash.h"
#include "AIController.h"
#include "EnemyCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

UBTTask_ChargeDash::UBTTask_ChargeDash()
{
	NodeName = TEXT("ChargeDash");
	INIT_TASK_NODE_NOTIFY_FLAGS();
}

EBTNodeResult::Type UBTTask_ChargeDash::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AEnemyCharacter* EnemyCharacter = Cast<AEnemyCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (!EnemyCharacter)
	{
		return EBTNodeResult::Failed;
	}
	ElapsedTime = 0.0f;
	EnemyCharacter->bIsCharging = true;
	EnemyCharacter->GetCharacterMovement()->MaxWalkSpeed = EnemyCharacter->ChargeSpeed;
	return EBTNodeResult::InProgress;
	
}

void UBTTask_ChargeDash::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	AEnemyCharacter* EnemyCharacter = Cast<AEnemyCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (!EnemyCharacter)
	{
		return;
	}
	if (EnemyCharacter->bDoorHitPending == true)
	{
		EnemyCharacter->bDoorHitPending = false;
		return FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
	else
	{
		EnemyCharacter->AddMovementInput(EnemyCharacter->ChargeDirection);
		ElapsedTime += DeltaSeconds;
	}
	if (ElapsedTime >= EnemyCharacter->ChargeDuration)
	{
		EnemyCharacter->GetCharacterMovement()->MaxWalkSpeed = EnemyCharacter->DefaultWalkSpeed;
		EnemyCharacter->ChargeCooldownRemaining = EnemyCharacter->ChargeCooldownDuration;
		EnemyCharacter->bIsCharging = false;
		return FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
