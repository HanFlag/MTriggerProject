// Fill out your copyright notice in the Description page of Project Settings.


#include "GA_EnemyCharge.h"

#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_WaitDelay.h"
#include "MomentTriggerGameplayTags.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_ApplyRootMotionConstantForce.h"
#include "GameFramework/RootMotionSource.h"
#include "Combat/AbilityTask_RotateToTarget.h"
#include "Kismet/GameplayStatics.h" 


void UGA_EnemyCharge::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                      const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                      const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return ;
	}
	if (!StartMontage)
	{
		OnStartAim();
		return;
	}
	UAbilityTask_PlayMontageAndWait* ReadyMontage = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, StartMontage);
	
	ReadyMontage->OnBlendOut.AddDynamic(this, &UGA_EnemyCharge::OnStartAim);
	ReadyMontage->OnInterrupted.AddDynamic(this, &UGA_EnemyCharge::OnChargeCancelled);
	ReadyMontage->OnCancelled.AddDynamic(this, &UGA_EnemyCharge::OnChargeCancelled);
	
	ReadyMontage->ReadyForActivation();
	
}

void UGA_EnemyCharge::OnStartAim()
{
	UAbilitySystemComponent* AvatarASC = GetAbilitySystemComponentFromActorInfo();
	if (!AvatarASC)
	{
		OnChargeCancelled();
		return;
	}
		AvatarASC->AddLooseGameplayTag(TAG_State_Charge_Aiming);
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (!PlayerPawn)
	{
		OnChargeCancelled();
		return;
	}
		RotateTask = UAbilityTask_RotateToTarget::RotateToTarget(this, PlayerPawn,WindupTurnRate);
		RotateTask->ReadyForActivation();
	UAbilityTask_WaitDelay* AimDelay = UAbilityTask_WaitDelay::WaitDelay(this, ChargeAimDuration);
	if (!AimDelay)
	{
		OnChargeCancelled();
		return;
	}
	AimDelay->OnFinish.AddDynamic(this, &UGA_EnemyCharge::OnAimFinished);
	AimDelay->ReadyForActivation();
}

void UGA_EnemyCharge::OnDashFinished()
{
	if (!StopMontage)
	{
		OnChargeEndMontageFinished();
		return;
	}
	UAbilityTask_PlayMontageAndWait* StopMT = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, StopMontage);
	if (!StopMT)
	{
		OnChargeEndMontageFinished();
		return;
	}
	StopMT->OnCompleted.AddDynamic(this, &UGA_EnemyCharge::OnChargeEndMontageFinished);
	StopMT->OnInterrupted.AddDynamic(this, &UGA_EnemyCharge::OnChargeEndMontageFinished);
	StopMT->OnCancelled.AddDynamic(this, &UGA_EnemyCharge::OnChargeEndMontageFinished);
	StopMT->ReadyForActivation();
	UAbilityTask_ApplyRootMotionConstantForce* SlideTask = UAbilityTask_ApplyRootMotionConstantForce::ApplyRootMotionConstantForce(this, NAME_None, ChargeDirection, ChargeSlideSpeed, SlideDuration, false, SlideCurve,ERootMotionFinishVelocityMode::SetVelocity, FVector::ZeroVector, 0.0f, true);
	SlideTask->ReadyForActivation();
}

void UGA_EnemyCharge::OnChargeEndMontageFinished()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UGA_EnemyCharge::OnChargeCancelled()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void UGA_EnemyCharge::OnAimFinished()
{
	UAbilitySystemComponent* AvatarASC = GetAbilitySystemComponentFromActorInfo();
	if (!AvatarASC)
	{
		OnChargeCancelled();
		return;
	}
	AvatarASC->SetLooseGameplayTagCount(TAG_State_Charge_Aiming, 0);
	if (RotateTask)
	{
		RotateTask->EndTask();
		RotateTask = nullptr;
	}
	AActor* Avatar = CurrentActorInfo->AvatarActor.Get();
	if (!Avatar)
	{
		OnChargeCancelled();
		return;
	}
	FVector TempForwardVector = Avatar->GetActorForwardVector();
	TempForwardVector.Z = 0.0f;
	ChargeDirection = TempForwardVector.GetSafeNormal();
	UAbilityTask_PlayMontageAndWait* LoopMontagePlay = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, LoopMontage);
	LoopMontagePlay->ReadyForActivation();
	UAbilityTask_ApplyRootMotionConstantForce* DashTask = UAbilityTask_ApplyRootMotionConstantForce::ApplyRootMotionConstantForce(this, NAME_None, ChargeDirection,ChargeSpeed, ChargeDuration, false, nullptr, ERootMotionFinishVelocityMode::MaintainLastRootMotionVelocity, FVector::ZeroVector, 0.0f, true);
	if (!DashTask)
	{
		OnChargeCancelled();
		return;
	}
	DashTask->OnFinish.AddDynamic(this, &UGA_EnemyCharge::OnDashFinished);
	DashTask->ReadyForActivation();	
}

void UGA_EnemyCharge::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	UAbilitySystemComponent* AvatarASC = GetAbilitySystemComponentFromActorInfo();
	if (AvatarASC)
	{
	AvatarASC->SetLooseGameplayTagCount(TAG_State_Charge_Aiming, 0);
	}
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
