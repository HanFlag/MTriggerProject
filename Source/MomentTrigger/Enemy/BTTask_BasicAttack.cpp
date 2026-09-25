// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_BasicAttack.h"

#include "AbilitySystemComponent.h"
#include "AIController.h"
#include "GameplayAbility.h"
#include "Kismet/GameplayStatics.h"
#include "Combat/GA_EnemyBasicAttack.h"
#include "Enemy/EnemyCharacter.h"

UBTTask_BasicAttack::UBTTask_BasicAttack()
{
	INIT_TASK_NODE_NOTIFY_FLAGS();
}

EBTNodeResult::Type UBTTask_BasicAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AEnemyCharacter* EnemyCharacter = Cast<AEnemyCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (!EnemyCharacter)
	{
		return EBTNodeResult::Failed;
	}
	if (!EnemyCharacter->BasicAttackClass)
	{
		return EBTNodeResult::Failed;
	}
	float AttackRange = EnemyCharacter->BasicAttackClass->GetDefaultObject<UGA_EnemyBasicAttack>()->AttackRange;
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (!PlayerPawn)
	{
		return EBTNodeResult::Failed;
	}
	if (FVector::Dist(EnemyCharacter->GetActorLocation(), PlayerPawn->GetActorLocation()) > AttackRange)
	{
		return EBTNodeResult::Failed;
	}
	bool bActivated = EnemyCharacter->AbilitySystemComp->TryActivateAbilityByClass(EnemyCharacter->BasicAttackClass);
	if (!bActivated)
	{
		return EBTNodeResult::Failed;
	}
	return EBTNodeResult::InProgress;
	
}

void UBTTask_BasicAttack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	AEnemyCharacter* EnemyCharacter = Cast<AEnemyCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (!EnemyCharacter)
	{
		return;
	}
	if (!EnemyCharacter->bIsAttacking)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
	
}
