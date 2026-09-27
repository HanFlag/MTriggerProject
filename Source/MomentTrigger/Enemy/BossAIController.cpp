// Fill out your copyright notice in the Description page of Project Settings.
#include "BossAIController.h"
#include "Components/StateTreeAIComponent.h"

ABossAIController::ABossAIController()
{
	StateTree = CreateDefaultSubobject<UStateTreeAIComponent>(TEXT("StateTreeAIComponent"));
	BrainComponent = StateTree;
	StateTree->SetStartLogicAutomatically(false);
}

void ABossAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	StateTree->StartLogic();
}
