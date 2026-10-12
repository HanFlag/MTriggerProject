// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "GameplayAbilitySpecHandle.h"
#include "Tasks/StateTreeAITask.h"
#include "STTask_AttackBurst.generated.h"

class AAIController;
class UGameplayAbility;

USTRUCT()
struct FSTTask_AttackBurstInstanceData
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category = "Context")
	TObjectPtr<AAIController> AIController = nullptr;
	UPROPERTY(EditAnywhere, Category = "Parameter")
	TSubclassOf<UGameplayAbility> AbilityClass;
	UPROPERTY(EditAnywhere, Category = "Parameter")
	int32 MinCount = 1;
	UPROPERTY(EditAnywhere, Category = "Parameter")
	int32 MaxCount = 3;
	UPROPERTY(EditAnywhere, Category = "Parameter")
	float MaxContinueDistance = 500.0f;
	UPROPERTY(EditAnywhere, Category = "Input")
	float DistanceToPlayer = 0.0f;
	
	FGameplayAbilitySpecHandle AbilitySpecHandle;
	int32 RemainingCount = 0;
};


USTRUCT(meta = (DisplayName = "Attack Burst", Category = "AI|Action"))
struct FSTTask_AttackBurst : public FStateTreeAIActionTaskBase
{
	GENERATED_BODY()
	using FInstanceDataType = FSTTask_AttackBurstInstanceData;
	virtual const UStruct* GetInstanceDataType() const override {return FInstanceDataType::StaticStruct();}
	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;
	virtual EStateTreeRunStatus Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const override;
	virtual void ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;

	
};
