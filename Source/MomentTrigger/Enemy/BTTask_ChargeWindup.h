// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_ChargeWindup.generated.h"

/**
 * 
 */
UCLASS()
class MOMENTTRIGGER_API UBTTask_ChargeWindup : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_ChargeWindup();
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
	//경과시간 저장 이 클래스 인스턴스는 여러 적이 동시에 공유할 수 있어서 보통은 NodeMemory에 저장해야 더 정확 하지만 테스트 이므로 단순 멤버 변수로 저장
	float ElapsedTime = 0.0f;
};
