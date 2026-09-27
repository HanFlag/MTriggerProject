// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HealthBarWidget.generated.h"

class UProgressBar;
class UAbilitySystemComponent;
struct FOnAttributeChangeData;

/**
 * 
 */
UCLASS()
class MOMENTTRIGGER_API UHealthBarWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
	UProgressBar* HealthBar;
	
	void InitWithASC(UAbilitySystemComponent* InASC);
protected:
	UPROPERTY()
	//최대 HP 읽기용
	UAbilitySystemComponent* ASC;
	void OnHealthChanged(const FOnAttributeChangeData& Data);
	void UpdateHealthBar(float CurrentHealth);
	
};
