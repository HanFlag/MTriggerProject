// Fill out your copyright notice in the Description page of Project Settings.


#include "GE_TestDamage.h"



UGE_TestDamage::UGE_TestDamage()
{
	DurationPolicy = EGameplayEffectDurationType::Instant;
	
	FGameplayModifierInfo ModifierInfo;
	ModifierInfo.Attribute = UMomentTriggerAttributeSet::GetHealthAttribute();
	ModifierInfo.ModifierOp = EGameplayModOp::Additive;
	ModifierInfo.ModifierMagnitude = FScalableFloat(-10.0f);
	
	Modifiers.Add(ModifierInfo);
}
