// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Tasks/StateTreeAITask.h"
#include "GameplayAbilitySpecHandle.h"
#include "STTask_ActivateAbility.generated.h"

class UGameplayAbility;
class AAIController;

/**
 * 
 */
USTRUCT()
struct FSTTask_ActivateAbilityInstanceData
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, Category = "Context")
	TObjectPtr<AAIController> AIController = nullptr;
	UPROPERTY(EditAnywhere, Category = "Parameter")
	TSubclassOf<UGameplayAbility> AbilityClass;
	
	FGameplayAbilitySpecHandle AbilitySpecHandle;
	
};

USTRUCT(meta = (DisplayName = "Activate Ability", Category = "AI|Action"))
struct FSTTask_ActivateAbility : public FStateTreeAIActionTaskBase
{
	GENERATED_BODY()
	using FInstanceDataType = FSTTask_ActivateAbilityInstanceData;
	virtual const UStruct* GetInstanceDataType() const override {return FInstanceDataType::StaticStruct();}
	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;
	virtual EStateTreeRunStatus Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const override;
	virtual void ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;
#if WITH_EDITOR
	virtual FText GetDescription(const FGuid& ID, FStateTreeDataView InstanceDataView, const IStateTreeBindingLookup& BindingLookup, EStateTreeNodeFormatting Formatting = EStateTreeNodeFormatting::Text) const override;
#endif
	
};
