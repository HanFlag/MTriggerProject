// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "MomentTriggerGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class MOMENTTRIGGER_API UMomentTriggerGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, Category = "Cooldown")
	// FScalableFloat 레벨별로 값을 다르게 줄 때 쓰는 타입
	FScalableFloat CooldownDuration = 0.0f;
	UPROPERTY(EditDefaultsOnly, Category = "Cooldown")
	FGameplayTagContainer CooldownTags;
	UPROPERTY(Transient)
	FGameplayTagContainer TempCooldownTags;
	
	virtual const FGameplayTagContainer* GetCooldownTags() const override;
	virtual void ApplyCooldown(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const override;
#if WITH_EDITOR
	virtual EDataValidationResult IsDataValid(class FDataValidationContext& Context) const override;
#endif
	
};
