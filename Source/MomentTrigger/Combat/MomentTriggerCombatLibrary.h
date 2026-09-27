// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MomentTriggerCombatLibrary.generated.h"

class UGameplayEffect;

/**
 * 
 */
UCLASS()
class MOMENTTRIGGER_API UMomentTriggerCombatLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	
	UFUNCTION(BlueprintCallable, Category = "Combat")
	static float ApplyDamage(AActor* DamageInstigator, AActor* Target, float DamageAmount, TSubclassOf<UGameplayEffect> DamageeffectClass);
	
};
