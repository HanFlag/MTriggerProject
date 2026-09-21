// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_DetectPlayer.generated.h"

/**
 * 
 */
UCLASS()
class MOMENTTRIGGER_API UBTService_DetectPlayer : public UBTService_BlackboardBase
{
	GENERATED_BODY()
	
public:
	
	UBTService_DetectPlayer();
	
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	//AI 적 감지 범위
	UPROPERTY(EditAnywhere, Category = "AI")
	float DetectionRadius = 800.0f;
};
