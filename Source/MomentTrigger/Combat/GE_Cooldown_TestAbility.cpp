// Fill out your copyright notice in the Description page of Project Settings.


#include "GE_Cooldown_TestAbility.h"

#include "GameplayEffectComponents/TargetTagsGameplayEffectComponent.h"
#include "MomentTriggerGameplayTags.h"

UGE_Cooldown_TestAbility::UGE_Cooldown_TestAbility()
{
	DurationPolicy = EGameplayEffectDurationType::HasDuration;
	DurationMagnitude = FGameplayEffectModifierMagnitude(FScalableFloat(0.05f));
}

void UGE_Cooldown_TestAbility::PostInitProperties()
{
	Super::PostInitProperties();

	UTargetTagsGameplayEffectComponent& TargetTagsComponent = FindOrAddComponent<UTargetTagsGameplayEffectComponent>();
	FInheritedTagContainer TagChanges;
	TagChanges.Added.AddTag(TAG_Cooldown_Ability_Test);
	TargetTagsComponent.SetAndApplyTargetTagChanges(TagChanges);
}
