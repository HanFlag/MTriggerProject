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
public:
	UGA_EnemyDodge();
protected:	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	UFUNCTION()
	void OnDodgeFinished();
	UFUNCTION()
	void OnDodgeCancelled();
	
	public:
	UPROPERTY(EditDefaultsOnly, Category = "Dodge")
	UAnimMontage* DodgeBackMontage;
	UPROPERTY(EditDefaultsOnly, Category = "Dodge")
	UAnimMontage* DodgeLeftMontage;
	UPROPERTY(EditDefaultsOnly, Category = "Dodge")
	UAnimMontage* DodgeRightMontage;
	UPROPERTY(EditDefaultsOnly, Category = "Dodge")
	float DodgeDistance = 400.0f;
	UPROPERTY(EditDefaultsOnly, Category = "Dodge")
	float MoveDuration = 0.6f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Dodge")
	int32 MaxChainCount = 2;
	UPROPERTY(EditDefaultsOnly, Category = "Dodge")
	float ChainCoolDownDuration = 8.0f;
	UPROPERTY(EditDefaultsOnly, Category = "Dodge")
	float ChainResetTime = 3.0f;
	UPROPERTY()
	int32 ChainCount = 0;
	UPROPERTY()
	float LastDodgeTime = -1000.0f;

	
		
	
	
protected:
	bool IsDodgePathClear(const FVector& Start, const FVector& End) const;
	void ApplyDodgeChainCoolDown(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const;
	
};
