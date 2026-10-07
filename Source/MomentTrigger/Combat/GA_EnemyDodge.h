// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Combat/MomentTriggerGameplayAbility.h"
#include "GA_EnemyDodge.generated.h"
class UAnimMontage;
/**
 * 
 */
UCLASS()
class MOMENTTRIGGER_API UGA_EnemyDodge : public UMomentTriggerGameplayAbility
{
	GENERATED_BODY()
protected:	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	UFUNCTION()
	void OnDodgeFinished();
	UFUNCTION()
	void OnDodgeCancelled();
	
	public:
	UPROPERTY(EditDefaultsOnly, Category = "Dodge")
	UAnimMontage* DodgeMontage;
	UPROPERTY(EditDefaultsOnly, Category = "Dodge")
	float DodgeDistance = 400.0f;
	UPROPERTY(EditDefaultsOnly, Category = "Dodge")
	float MoveDuration = 0.6f;
	
	
};
