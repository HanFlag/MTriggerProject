// Fill out your copyright notice in the Description page of Project Settings.


#include "STTask_ActivateAbility.h"

#include "StateTreeExecutionContext.h"
#include "AIController.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "Abilities/GameplayAbility.h"
#define LOCTEXT_NAMESPACE "STTask_ActivateAbility"



static UAbilitySystemComponent* GetASCFromActivateAbilityData(const FSTTask_ActivateAbilityInstanceData& Data)
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

EStateTreeRunStatus FSTTask_ActivateAbility::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const
{
	FInstanceDataType& InstanceData = Context.GetInstanceData(*this);
	UAbilitySystemComponent* PawnASC = GetASCFromActivateAbilityData(InstanceData);
	if (!PawnASC)
	{
		return EStateTreeRunStatus::Failed;
	}
	FGameplayAbilitySpec* Spec = PawnASC->FindAbilitySpecFromClass(InstanceData.AbilityClass);
	if (!Spec)
	{
		return EStateTreeRunStatus::Failed;
	}
	if (PawnASC->TryActivateAbility(Spec->Handle))
	{
		InstanceData.AbilitySpecHandle = Spec->Handle;
		return EStateTreeRunStatus::Running;
	}
	else
	{
		return EStateTreeRunStatus::Failed;
	}
}

EStateTreeRunStatus FSTTask_ActivateAbility::Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const
{
	FInstanceDataType& InstanceData = Context.GetInstanceData(*this);
	UAbilitySystemComponent* PawnASC = GetASCFromActivateAbilityData(InstanceData);
	if (!PawnASC)
	{
		return EStateTreeRunStatus::Failed;
	}
	FGameplayAbilitySpec* Spec = PawnASC->FindAbilitySpecFromHandle(InstanceData.AbilitySpecHandle);
	if (!Spec || !Spec->IsActive())
	{
		return EStateTreeRunStatus::Succeeded;
	}
	return EStateTreeRunStatus::Running;
	
	
}

void FSTTask_ActivateAbility::ExitState(FStateTreeExecutionContext& Context,
                                        const FStateTreeTransitionResult& Transition) const
{
	FInstanceDataType& InstanceData = Context.GetInstanceData(*this);
	UAbilitySystemComponent* PawnASC = GetASCFromActivateAbilityData(InstanceData);
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
#if WITH_EDITOR
FText FSTTask_ActivateAbility::GetDescription(const FGuid& ID, FStateTreeDataView InstanceDataView,
	const IStateTreeBindingLookup& BindingLookup, EStateTreeNodeFormatting Formatting) const
{
	
	const FInstanceDataType* InstanceData = InstanceDataView.GetPtr<FInstanceDataType>();
	check(InstanceData);
	FString AbilityName = TEXT("None");
	if (InstanceData->AbilityClass)
	{
		AbilityName = InstanceData->AbilityClass->GetName();
		AbilityName.RemoveFromEnd(TEXT("_C"));
	}
	if (Formatting == EStateTreeNodeFormatting::RichText)
	{
		return FText::Format(LOCTEXT("ActivateAbilityDescRich", "<b>Activate:</> {0}"), FText::FromString(AbilityName));
	}
	return FText::Format(LOCTEXT("ActivateAbilityDesc", "Activate: {0}"), FText::FromString(AbilityName));
	
}
#endif

#undef LOCTEXT_NAMESPACE