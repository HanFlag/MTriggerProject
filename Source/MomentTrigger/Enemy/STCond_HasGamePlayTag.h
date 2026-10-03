// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "Conditions/StateTreeAIConditionBase.h"
#include "GameplayTagContainer.h"
#include "STCond_HasGamePlayTag.generated.h"


USTRUCT()
struct FSTCond_HasGamePlayTagInstanceData
{
	GENERATED_BODY()
 	
public:
	
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<AActor> Actor = nullptr;
	UPROPERTY(EditAnywhere, Category = "Parameter")
	FGameplayTag Tag;


};

USTRUCT(meta = (DisplayName = "Has Gameplay Tag", Category = "AI"))
struct FSTCond_HasGameplayTag : public FStateTreeAIConditionBase
{
	GENERATED_BODY()
	
public:
	using FInstanceDataType = FSTCond_HasGamePlayTagInstanceData;
	virtual const UStruct* GetInstanceDataType() const override;
	virtual bool TestCondition(FStateTreeExecutionContext& Context) const override;
	UPROPERTY(EditAnywhere, Category = "Parameter")
	bool bInvert = false;
	
	
public:
#if WITH_EDITOR
	virtual FText GetDescription(const FGuid& ID, FStateTreeDataView InstanceDataView, const IStateTreeBindingLookup& BindingLookup, EStateTreeNodeFormatting Formatting = EStateTreeNodeFormatting::Text) const override;
#endif
	
};
