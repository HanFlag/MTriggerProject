// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ABasicAttackProjectile.h"
#include "Animation/AnimInstance.h"
#include "Abilities/GameplayAbility.h"
#include "MomentTriggerGameplayTags.h"
#include "GA_BasicAttack.generated.h"

UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Event_BasicAttack_Hit)

/**
 * 
 */
UCLASS()
class MOMENTTRIGGER_API UGA_BasicAttack : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
	UPROPERTY(EditDefaultsOnly, Category = "Ability")
	UAnimMontage* AttackMontage;
	
	UPROPERTY(EditDefaultsOnly, Category = "Ability")
	FName HitNotifyName = TEXT("Hit");
	
	UPROPERTY(EditDefaultsOnly, Category = "Ability")
	TSubclassOf<AABasicAttackProjectile> ProjectileClass;
	
	
	UFUNCTION()
	void OnHitEventReceived (FGameplayEventData Payload);
	UFUNCTION()
	void OnMontageComplated();
	
	
	
	
};
