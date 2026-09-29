// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_ActivateAbility.h"
#include "AIController.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "Abilities/GameplayAbility.h"



UBTTask_ActivateAbility::UBTTask_ActivateAbility()
{
	
	INIT_TASK_NODE_NOTIFY_FLAGS();
	NodeName = TEXT("Activate Ability");
}

EBTNodeResult::Type UBTTask_ActivateAbility::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UAbilitySystemComponent* EnemyASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(OwnerComp.GetAIOwner()->GetPawn());
	if (!EnemyASC || !AbilityClass)
	{
		return EBTNodeResult::Failed;
	}
	if (EnemyASC->TryActivateAbilityByClass(AbilityClass) == false)
	{
		return EBTNodeResult::Failed;
	}
	return EBTNodeResult::InProgress;
}

EBTNodeResult::Type UBTTask_ActivateAbility::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UAbilitySystemComponent* EnemyASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(OwnerComp.GetAIOwner()->GetPawn());
	if (!EnemyASC)
	{
		return EBTNodeResult::Aborted;
	}
	FGameplayAbilitySpec* AbilitySpec = EnemyASC->FindAbilitySpecFromClass(AbilityClass);
	if (AbilitySpec)
	{
		EnemyASC->CancelAbilityHandle(AbilitySpec->Handle);
		return EBTNodeResult::Aborted;
	}
	
	return EBTNodeResult::Aborted;
}

void UBTTask_ActivateAbility::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	UAbilitySystemComponent* EnemyASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(OwnerComp.GetAIOwner()->GetPawn());
	if (!EnemyASC)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}
	FGameplayAbilitySpec* AbilitySpec = EnemyASC->FindAbilitySpecFromClass(AbilityClass);
	if (!AbilitySpec || !AbilitySpec->IsActive())
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return;
	}
}
