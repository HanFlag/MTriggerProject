// Fill out your copyright notice in the Description page of Project Settings.


#include "MomentTriggerGameplayAbility.h"
#include "MomentTriggerGameplayTags.h"

const FGameplayTagContainer* UMomentTriggerGameplayAbility::GetCooldownTags() const
{
	
	FGameplayTagContainer* MutableTags = const_cast<FGameplayTagContainer*> (&TempCooldownTags);
	MutableTags->Reset();
	const FGameplayTagContainer* ParentTags = Super::GetCooldownTags();
	if (ParentTags)
	{
		MutableTags->AppendTags(*ParentTags);
	}
	MutableTags->AppendTags(CooldownTags);
	return MutableTags;
}

void UMomentTriggerGameplayAbility::ApplyCooldown(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const
{
	UGameplayEffect* CooldownGE = GetCooldownGameplayEffect();
	if (!CooldownGE)
	{
		return;
	}
	FGameplayEffectSpecHandle SpecHandle = MakeOutgoingGameplayEffectSpec(CooldownGE->GetClass(), GetAbilityLevel());
	if (SpecHandle.IsValid())
	{
		SpecHandle.Data->DynamicGrantedTags.AppendTags(CooldownTags);
		SpecHandle.Data->SetSetByCallerMagnitude(TAG_Data_Cooldown, CooldownDuration.GetValueAtLevel(GetAbilityLevel()));
		ApplyGameplayEffectSpecToOwner(Handle, ActorInfo, ActivationInfo, SpecHandle);
	}
}
#if WITH_EDITOR
#include "Misc/DataValidation.h"
EDataValidationResult UMomentTriggerGameplayAbility::IsDataValid(class FDataValidationContext& Context) const
{
	EDataValidationResult Result = Super::IsDataValid(Context);
	if (CooldownGameplayEffectClass)
	{
		if (CooldownTags.IsEmpty())
		{
			Context.AddError(FText::FromString(TEXT("Cooldown GE is set but CooldownTags is empty")));
			Result = EDataValidationResult::Invalid;
		}
		if (CooldownDuration.GetValueAtLevel(1) <= 0.0f)
		{
			Context.AddError(FText::FromString(TEXT("Cooldown GE is set but CooldownDuration is <= 0")));
			Result = EDataValidationResult::Invalid;
		}
	}
	return Result;
}
#endif
