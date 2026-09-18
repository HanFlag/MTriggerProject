// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilitySystemBlueprintLibrary.h"
#include "MomentTriggerGameplayTags.h"
#include "AnimNotify_BasicAttackHit.h"

#include "GA_BasicAttack.h"

void UAnimNotify_BasicAttackHit::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                        const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	
	if (AActor* Owner = MeshComp->GetOwner())
	{
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Owner, TAG_Event_BasicAttack_Hit, FGameplayEventData());
	}
}
