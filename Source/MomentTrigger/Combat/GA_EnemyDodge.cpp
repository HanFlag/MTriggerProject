// Fill out your copyright notice in the Description page of Project Settings.


#include "GA_EnemyDodge.h"

#include "Abilities/Tasks/AbilityTask_ApplyRootMotionMoveToForce.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "GameFramework/Character.h"

void UGA_EnemyDodge::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	if (!DodgeMontage || !CommitAbility( Handle, ActorInfo, ActivationInfo) )
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}
	ACharacter* Avatar = Cast<ACharacter>(ActorInfo->AvatarActor.Get());
	if (!Avatar)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}
	FVector TargetPoint = Avatar->GetActorLocation() - Avatar->GetActorForwardVector() * DodgeDistance;
	
	UAbilityTask_PlayMontageAndWait* DodgeTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, DodgeMontage);
	if (!DodgeTask)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}
	DodgeTask->OnCancelled.AddDynamic(this, &UGA_EnemyDodge::OnDodgeCancelled);
	DodgeTask->OnCompleted.AddDynamic(this, &UGA_EnemyDodge::OnDodgeFinished);
	DodgeTask->OnInterrupted.AddDynamic(this, &UGA_EnemyDodge::OnDodgeCancelled);
	DodgeTask->OnBlendOut.AddDynamic(this, &UGA_EnemyDodge::OnDodgeFinished);
	UAbilityTask_ApplyRootMotionMoveToForce* DodgeMoveTask = UAbilityTask_ApplyRootMotionMoveToForce::ApplyRootMotionMoveToForce(this, NAME_None, TargetPoint, MoveDuration, false, MOVE_Walking, false, nullptr, ERootMotionFinishVelocityMode::SetVelocity, FVector::ZeroVector, 0.0f);
	if (!DodgeMoveTask)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}
	DodgeMoveTask->ReadyForActivation();
	DodgeTask->ReadyForActivation();
}

void UGA_EnemyDodge::OnDodgeFinished()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UGA_EnemyDodge::OnDodgeCancelled()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}
