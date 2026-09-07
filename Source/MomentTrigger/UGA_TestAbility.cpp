// Fill out your copyright notice in the Description page of Project Settings.


#include "UGA_TestAbility.h"

UUGA_TestAbility::UUGA_TestAbility()
{
	AbilityTags.AddTag(TAG_Ability_Test);
}

void UUGA_TestAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                       const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                       const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	UE_LOG(LogTemp, Warning, TEXT("Ability Activated"));
	EndAbility(Handle, ActorInfo, ActivationInfo, true,false);
}


//UE_LOG(LogTemp, Warning, TEXT("TestAbility Activated! Tags: %s"), *AbilityTags.ToString());