// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BossAIController.generated.h"

class UStateTreeAIComponent;

/**
 * 
 */
UCLASS()
class MOMENTTRIGGER_API ABossAIController : public AAIController
{
	GENERATED_BODY()
public:
	ABossAIController();	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	UStateTreeAIComponent* StateTree;
	
	virtual void OnPossess(APawn* InPawn) override;
};
