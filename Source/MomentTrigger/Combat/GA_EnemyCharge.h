// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MomentTriggerGameplayAbility.h"
#include "GA_EnemyCharge.generated.h"

class UAnimMontage;
class UGameplayEffect;
class UCurveFloat;
/**
 * 
 */
UCLASS()
class MOMENTTRIGGER_API UGA_EnemyCharge : public UMomentTriggerGameplayAbility
{
	GENERATED_BODY()
protected:
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
public:
	UPROPERTY(EditDefaultsOnly, Category = "Charge")
	float ChargeSpeed = 1500.0f;
	UPROPERTY(EditDefaultsOnly, Category = "Charge")
	float ChargeSlideSpeed = 600.0f;
	UPROPERTY(EditDefaultsOnly, Category = "Charge")
	float ChargeDuration = 1.0f;
	UPROPERTY(EditDefaultsOnly, Category = "Charge")
	float ChargeDamage = 30.0f;
	UPROPERTY(EditDefaultsOnly, Category = "Charge")
	float SlideDuration = 0.5f;
	UPROPERTY(EditDefaultsOnly, Category = "Charge")
	UCurveFloat* SlideCurve;
	UPROPERTY(EditDefaultsOnly, Category = "Charge")
	bool bStopChargeOnHitPlayer = true;
	UPROPERTY(EditDefaultsOnly, Category = "Charge")
	TSubclassOf<UGameplayEffect> DamageEffectClass;
	UPROPERTY(EditDefaultsOnly, Category = "Charge")
	UAnimMontage* StartMontage;
	UPROPERTY(EditDefaultsOnly, Category = "Charge")
	UAnimMontage* LoopMontage;
	UPROPERTY(EditDefaultsOnly, Category = "Charge")
	UAnimMontage* StopMontage;
	UPROPERTY(EditDefaultsOnly, Category = "Charge")
	UAnimMontage* KnockDownMontage;
	UPROPERTY(EditDefaultsOnly, Category = "Charge")
	UAnimMontage* GetUpMontage;
	
	
	UFUNCTION()
	void OnWindupBlendOut();
	UFUNCTION()
	void OnDashFinished();
	UFUNCTION()
	void OnChargeEndMontageFinished();
	UFUNCTION()
	void OnChargeCancelled();
	
protected:
	FVector ChargeDirection;
	
	
	

	
};
