// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AbilityTask_RotateToTarget.generated.h"

/**
 * 
 */
UCLASS()
class MOMENTTRIGGER_API UAbilityTask_RotateToTarget : public UAbilityTask
{
	GENERATED_BODY()
public:
	UAbilityTask_RotateToTarget(const FObjectInitializer& ObjectInitializer);
	
	
	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UAbilityTask_RotateToTarget* RotateToTarget(UGameplayAbility* OwningAbility,AActor* InTarget,float InTurnRate);
	
	virtual void TickTask(float DeltaTime) override;
	
protected:
	TWeakObjectPtr<AActor> Target;
	//초당 회전각도
	float TurnRate = 60.0f;
};
