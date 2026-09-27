// Fill out your copyright notice in the Description page of Project Settings.

#include "HealthBarWidget.h"
#include "Components/ProgressBar.h"
#include "AbilitySystemComponent.h"
#include "Combat/MomentTriggerAttributeSet.h"

void UHealthBarWidget::InitWithASC(UAbilitySystemComponent* InASC)
{
	ASC=InASC;
	if (!ASC)
	{
		return;
	}
	ASC->GetGameplayAttributeValueChangeDelegate(UMomentTriggerAttributeSet::GetHealthAttribute()).AddUObject(this, &UHealthBarWidget::OnHealthChanged);
	UpdateHealthBar(ASC->GetNumericAttribute(UMomentTriggerAttributeSet::GetHealthAttribute()));
}

void UHealthBarWidget::OnHealthChanged(const FOnAttributeChangeData& Data)
{
	UpdateHealthBar(Data.NewValue);
}

void UHealthBarWidget::UpdateHealthBar(float CurrentHealth)
{
	if (HealthBar && ASC)
	{
		float MaxHealth = ASC->GetNumericAttribute(UMomentTriggerAttributeSet::GetMaxHealthAttribute());
		if (MaxHealth > 0.0f)
		{
			HealthBar->SetPercent(CurrentHealth / MaxHealth);
		}
	}
	
}
