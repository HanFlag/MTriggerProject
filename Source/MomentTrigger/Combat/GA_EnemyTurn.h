// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Combat/MomentTriggerGameplayAbility.h"
#include "GA_EnemyTurn.generated.h"

class UAnimMontage;

/**
 * 
 */
UCLASS()
class MOMENTTRIGGER_API UGA_EnemyTurn : public UMomentTriggerGameplayAbility
{
	GENERATED_BODY()
protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	public:
	UPROPERTY(EditDefaultsOnly, Category = "TurnWarping")
	UAnimMontage* TurnL90Montage;
	UPROPERTY(EditDefaultsOnly, Category = "TurnWarping")
	UAnimMontage* TurnL180Montage;
	UPROPERTY(EditDefaultsOnly, Category = "TurnWarping")
	UAnimMontage* TurnR90Montage;
	UPROPERTY(EditDefaultsOnly, Category = "TurnWarping")
	UAnimMontage* TurnR180Montage;
	UPROPERTY(EditDefaultsOnly, Category = "TurnWarping")
	float MinTurnAngle = 45.0f;
	UPROPERTY(EditDefaultsOnly, Category = "TurnWarping")
	float Use180Angle = 135.0f;
	UPROPERTY(EditDefaultsOnly, Category = "TurnWarping")
	FName WarpTargetName = TEXT("TurnTarget");
	
	
	UFUNCTION()
	void OnTurnMontageFinished();
};
