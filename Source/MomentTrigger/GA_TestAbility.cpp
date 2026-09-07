// Fill out your copyright notice in the Description page of Project Settings.


#include "GA_TestAbility.h"

#include "GE_Cooldown_TestAbility.h"
#include "GE_TestDamage.h"
#include "MomentTriggerAttributeSet.h"

UGA_TestAbility::UGA_TestAbility()
{
	
	AbilityTags.AddTag(TAG_Ability_Test);
	SetAssetTags(AbilityTags);
	TestDamageEffectClass = UGE_TestDamage::StaticClass();
	CooldownGameplayEffectClass = UGE_Cooldown_TestAbility::StaticClass();
}

void UGA_TestAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                       const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                       const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get();
	const UMomentTriggerAttributeSet* AttributeSet = ASC ? ASC->GetSet<UMomentTriggerAttributeSet>() : nullptr;

	if (AttributeSet)
	{
		UE_LOG(LogTemp, Warning, TEXT("Health Before: %f"), AttributeSet->GetHealth());
	}
	if (ASC && TestDamageEffectClass)
	{
		FGameplayEffectContextHandle ContextHandle = ASC->MakeEffectContext();
		FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(TestDamageEffectClass, 1.0f, ContextHandle);
		if (SpecHandle.IsValid())
		{
			ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
		}
		
		if (AttributeSet)
		{
			UE_LOG(LogTemp, Warning, TEXT("Health After: %f"), AttributeSet->GetHealth());
		}
		
	}
	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}
