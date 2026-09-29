// Fill out your copyright notice in the Description page of Project Settings.


#include "GA_EnemyCharge.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_ApplyRootMotionConstantForce.h"
#include "GameFramework/RootMotionSource.h"
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
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}
	UAbilityTask_PlayMontageAndWait* ReadyMontage = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, StartMontage);
	
	ReadyMontage->OnBlendOut.AddDynamic(this, &UGA_EnemyCharge::OnWindupBlendOut);
	ReadyMontage->OnInterrupted.AddDynamic(this, &UGA_EnemyCharge::OnChargeCancelled);
	ReadyMontage->OnCancelled.AddDynamic(this, &UGA_EnemyCharge::OnChargeCancelled);
	
	ReadyMontage->ReadyForActivation();
}

void UGA_EnemyCharge::OnWindupBlendOut()
{
	AActor* Avatar = CurrentActorInfo->AvatarActor.Get();
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (!Avatar || !PlayerPawn)
	{
		OnChargeCancelled();
		return;
	}
	FVector TempChargeDirection = PlayerPawn->GetActorLocation() - Avatar->GetActorLocation();
	TempChargeDirection.Z = 0.0f;
	ChargeDirection = TempChargeDirection.GetSafeNormal();
	Avatar->SetActorRotation(ChargeDirection.Rotation());
	UAbilityTask_PlayMontageAndWait* LoopMontagePlay = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, LoopMontage);
	LoopMontagePlay->ReadyForActivation();
	UAbilityTask_ApplyRootMotionConstantForce* DashTask = UAbilityTask_ApplyRootMotionConstantForce::ApplyRootMotionConstantForce(this, NAME_None, ChargeDirection,ChargeSpeed, ChargeDuration, false, nullptr, ERootMotionFinishVelocityMode::ClampVelocity, FVector::ZeroVector, ChargeSlideSpeed, true);
	if (!DashTask)
	{
		OnChargeCancelled();
		return;
	}
	DashTask->OnFinish.AddDynamic(this, &UGA_EnemyCharge::OnDashFinished);
	DashTask->ReadyForActivation();
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
}

void UGA_EnemyCharge::OnChargeEndMontageFinished()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UGA_EnemyCharge::OnChargeCancelled()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}
