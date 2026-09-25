// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GA_EnemyBasicAttack.generated.h"

/**
 * 
 */
UCLASS()
class MOMENTTRIGGER_API UGA_EnemyBasicAttack : public UGameplayAbility
{
	GENERATED_BODY()
	
	public:
	UPROPERTY(EditDefaultsOnly, Category = "Ability")
	TArray<UAnimMontage*> AttackMontages;
	UPROPERTY(EditDefaultsOnly, Category = "Ability")
	TSubclassOf<UGameplayEffect> DamageEffectClass;
	UPROPERTY(EditDefaultsOnly, Category = "Ability")
	float AttackDamage = 15.0f;
	UPROPERTY(EditDefaultsOnly, Category = "Ability")
	float AttackRange = 150.0f;
	
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
	UFUNCTION()
	void OnHitEventReceived(FGameplayEventData Payload);
	UFUNCTION()
	void OnMontageCompleted();
	
	
};
