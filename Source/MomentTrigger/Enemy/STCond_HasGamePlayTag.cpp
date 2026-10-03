
#include "STCond_HasGamePlayTag.h"
#include "StateTreeExecutionContext.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#define LOCTEXT_NAMESPACE "STCond_HasGamePlayTag"

const UStruct* FSTCond_HasGameplayTag::GetInstanceDataType() const
{
	return FInstanceDataType::StaticStruct();
}

bool FSTCond_HasGameplayTag::TestCondition(FStateTreeExecutionContext& Context) const
{
	const FInstanceDataType& InstanceData = Context.GetInstanceData(*this);
	
	UAbilitySystemComponent* ActorASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(InstanceData.Actor);
	if (!ActorASC)
	{
		return false;
	}
	const bool bHasTag = ActorASC->HasMatchingGameplayTag(InstanceData.Tag);
	
	
	return bHasTag != bInvert;
}



#if WITH_EDITOR
// 태그 체크 알림 기능
FText FSTCond_HasGameplayTag::GetDescription(const FGuid& ID, FStateTreeDataView InstanceDataView,
	const IStateTreeBindingLookup& BindingLookup, EStateTreeNodeFormatting Formatting) const
{
	
	const FInstanceDataType* InstanceData = InstanceDataView.GetPtr<FInstanceDataType>();
	check(InstanceData);
	FString TagName = TEXT("None");
	if (InstanceData->Tag.IsValid())
	{
	TagName = InstanceData->Tag.ToString();
	}
	const FText Prefix = bInvert ? LOCTEXT("HasNotTag", "Not") : LOCTEXT("HasTag", "Has");
	return FText::Format(LOCTEXT("HasTagDesc", "{0} {1}"), Prefix, FText::FromString(TagName));
}
#endif

#undef LOCTEXT_NAMESPACE