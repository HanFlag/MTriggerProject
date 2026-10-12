#include "STTask_AttackBurst.h"
#include "AIController.h"
#include "StateTreeExecutionContext.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "Abilities/GameplayAbility.h"

static UAbilitySystemComponent* GetASCFromAttackBurstData(const FSTTask_AttackBurstInstanceData& Data)
{
	
	if (!Data.AIController)
	{
		return nullptr;
	}
	APawn* Pawn = Data.AIController->GetPawn();
	if (!Pawn)
	{
		return nullptr;
	}
	return UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Pawn);
}


EStateTreeRunStatus FSTTask_AttackBurst::EnterState(FStateTreeExecutionContext& Context,
	const FStateTreeTransitionResult& Transition) const
{
	FInstanceDataType& InstanceData = Context.GetInstanceData(*this);
	UAbilitySystemComponent* PawnASC = GetASCFromAttackBurstData(InstanceData);
	if (!PawnASC)
	{
		return EStateTreeRunStatus::Failed;
	}
	FGameplayAbilitySpec* Spec = PawnASC->FindAbilitySpecFromClass(InstanceData.AbilityClass);
	if (!Spec)
	{
		return EStateTreeRunStatus::Failed;
	}
	
	InstanceData.RemainingCount = FMath::RandRange(InstanceData.MinCount, InstanceData.MaxCount);
	if (PawnASC->TryActivateAbility(Spec->Handle))
	{
		InstanceData.RemainingCount--;
		InstanceData.AbilitySpecHandle = Spec->Handle;
		return EStateTreeRunStatus::Running;
	}
	else
	{
		return EStateTreeRunStatus::Failed;
	}
}

EStateTreeRunStatus FSTTask_AttackBurst::Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const
{
	FInstanceDataType& InstanceData = Context.GetInstanceData(*this);
	UAbilitySystemComponent* PawnASC = GetASCFromAttackBurstData(InstanceData);
	if (!PawnASC)
	{
		return EStateTreeRunStatus::Failed;
	}
	FGameplayAbilitySpec* Spec = PawnASC->FindAbilitySpecFromHandle(InstanceData.AbilitySpecHandle);
	if (!Spec)
	{
		return EStateTreeRunStatus::Failed;
	}
	if (Spec->IsActive())
	{
		return EStateTreeRunStatus::Running;
	}

	if (InstanceData.RemainingCount <= 0)
	{
		return EStateTreeRunStatus::Succeeded;
	}
	if (InstanceData.DistanceToPlayer > InstanceData.MaxContinueDistance)
	{
		return EStateTreeRunStatus::Succeeded;
	}
	if (!PawnASC->TryActivateAbility(Spec->Handle))
	{
		return EStateTreeRunStatus::Succeeded;
	}
		InstanceData.RemainingCount--;
		return EStateTreeRunStatus::Running;

}

void FSTTask_AttackBurst::ExitState(FStateTreeExecutionContext& Context,
	const FStateTreeTransitionResult& Transition) const
{
	FInstanceDataType& InstanceData = Context.GetInstanceData(*this);
	UAbilitySystemComponent* PawnASC = GetASCFromAttackBurstData(InstanceData);
	if (!PawnASC)
	{
		return;
	}
	FGameplayAbilitySpec* Spec = PawnASC->FindAbilitySpecFromHandle(InstanceData.AbilitySpecHandle);
	if (Spec && Spec->IsActive())
	{
		PawnASC->CancelAbilityHandle(InstanceData.AbilitySpecHandle);
	}
	FStateTreeAIActionTaskBase::ExitState(Context, Transition);
}
