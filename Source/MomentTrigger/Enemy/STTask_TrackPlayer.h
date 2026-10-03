// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Tasks/StateTreeAITask.h"
#include "STTask_TrackPlayer.generated.h"

class AAIController;
class AActor;

/**
 * 
 */

USTRUCT()
struct FSTTask_TrackPlayerInstanceData
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, Category = "Context")
	TObjectPtr<AAIController> AIController = nullptr;
	UPROPERTY(EditAnywhere, Category = "Output")
	TObjectPtr<AActor> PlayerActor = nullptr;
	UPROPERTY(EditAnywhere, Category = "Output")
	float DistanceToPlayer = 0.0f;
	UPROPERTY(EditAnywhere, Category = "Output")
	bool bPlayerAlive = false;
};

USTRUCT(meta = (DisplayName = "Track Player", Category = "AI"))
struct FSTTask_TrackPlayer : public FStateTreeAITaskBase
{
	GENERATED_BODY()
	public:
	using FInstanceDataType = FSTTask_TrackPlayerInstanceData;
		virtual const UStruct* GetInstanceDataType() const override;
	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;
	virtual EStateTreeRunStatus Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const override;
};
