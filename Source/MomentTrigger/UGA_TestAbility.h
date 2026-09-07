// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "MomentTriggerGameplayTags.h"
#include "UGA_TestAbility.generated.h"


/**
 * 
 */
UCLASS()
class MOMENTTRIGGER_API UUGA_TestAbility : public UGameplayAbility
{
	GENERATED_BODY()
	
	UUGA_TestAbility();
	
public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
};
