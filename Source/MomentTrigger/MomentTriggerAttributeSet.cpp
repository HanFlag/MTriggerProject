// Fill out your copyright notice in the Description page of Project Settings.


#include "MomentTriggerAttributeSet.h"

#include "Net/UnrealNetwork.h"

UMomentTriggerAttributeSet::UMomentTriggerAttributeSet()
{
	Health.SetBaseValue(100.0f);
	Health.SetCurrentValue(100.0f);
	
}

void UMomentTriggerAttributeSet::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME_CONDITION_NOTIFY(UMomentTriggerAttributeSet, Health, COND_None, REPNOTIFY_Always);
	
}

void UMomentTriggerAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMomentTriggerAttributeSet,Health,OldHealth);
}
