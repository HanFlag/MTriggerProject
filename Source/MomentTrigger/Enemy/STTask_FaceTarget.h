// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Tasks/StateTreeAITask.h"
#include "STTask_FaceTarget.generated.h"
class AAIController;

USTRUCT()
struct FSTTask_FaceTargetInstanceData
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category = "Context")
	TObjectPtr<AAIController> AIController = nullptr;
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<AActor> Target = nullptr;
};


USTRUCT(meta = (DisplayName = "Face Target", Category = "AI|Action"))
struct FSTTask_FaceTarget : public FStateTreeAITaskBase
{
	GENERATED_BODY()
	
public:
	FSTTask_FaceTarget();
	using FInstanceDataType = FSTTask_FaceTargetInstanceData;
	virtual const UStruct* GetInstanceDataType() const override;
	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;
	virtual void ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;
	
};