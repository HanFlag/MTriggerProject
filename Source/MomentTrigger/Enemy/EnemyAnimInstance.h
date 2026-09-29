// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "Animation/AnimInstance.h"
#include "EnemyAnimInstance.generated.h"
class UAbilitySystemComponent;
/**
 * 
 */


UCLASS()
class MOMENTTRIGGER_API UEnemyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
protected:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	
	UPROPERTY(EditDefaultsOnly, Category = "GameplayTags")
	FGameplayTagBlueprintPropertyMap GameplayTagPropertyMap;
	UPROPERTY(BlueprintReadOnly, Category = "Charge")
	bool bIsChargeAiming = false;
	UPROPERTY(BlueprintReadOnly, Category = "Charge")
	float AimYawOffset = 0.0f;
	UPROPERTY(BlueprintReadOnly, Category = "Charge")
	FVector LookAtLocation = FVector::ZeroVector;
	UPROPERTY(BlueprintReadOnly, Category = "Locomotion")
	float Speed = 0.0f;
	
	UPROPERTY()
	UAbilitySystemComponent* OwnerASC;
	
#if WITH_EDITOR
	virtual EDataValidationResult IsDataValid(class FDataValidationContext& Context) const override;
#endif
};
