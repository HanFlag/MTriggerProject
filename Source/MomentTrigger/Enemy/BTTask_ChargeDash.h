// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_ChargeDash.generated.h"

/**
 * 
 */
UCLASS()
class MOMENTTRIGGER_API UBTTask_ChargeDash : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTTask_ChargeDash();
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
	float ElapsedTime = 0.0f;
};
