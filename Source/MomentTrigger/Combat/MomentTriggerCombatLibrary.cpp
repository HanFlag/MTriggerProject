// Fill out your copyright notice in the Description page of Project Settings.


#include "MomentTriggerCombatLibrary.h"
#include "Engine/DamageEvents.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "MomentTriggerGameplayTags.h"

float UMomentTriggerCombatLibrary::ApplyDamage(AActor* DamageInstigator, AActor* Target, float DamageAmount,
                                               TSubclassOf<UGameplayEffect> DamageeffectClass)
{
	if (!DamageInstigator || !Target || DamageAmount
		<= 0.0f || Target == DamageInstigator)
	{
		return 0.0f;
	}
	UAbilitySystemComponent* TargetASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Target);
	if (TargetASC)
	{
		UAbilitySystemComponent* SourceASC =
			UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(DamageInstigator);
		UAbilitySystemComponent* SpecASC = SourceASC ? SourceASC : TargetASC;
		FGameplayEffectContextHandle ContextHandle = SpecASC->MakeEffectContext();
		if (!SourceASC)
		{
			ContextHandle.AddInstigator(DamageInstigator,DamageInstigator);
		}
		FGameplayEffectSpecHandle SpecHandle = SpecASC->MakeOutgoingSpec(DamageeffectClass, 1.0f, ContextHandle);
		if (!SpecHandle.IsValid())
		{
			UE_LOG(LogTemp, Warning, TEXT("DamageEffectClass is not valid"));
		}
		else
		{
			SpecHandle.Data->SetSetByCallerMagnitude(TAG_Data_Damage, -DamageAmount);
			SpecASC->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), TargetASC);
			return DamageAmount;
		}
		return 0.0f;
	}
	APawn* Pawn = Cast<APawn>(DamageInstigator);
	AController* InstigatorController = Pawn ? Pawn->GetController() : nullptr;
	return Target->TakeDamage(DamageAmount, FDamageEvent(), InstigatorController, DamageInstigator);
}
