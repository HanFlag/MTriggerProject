// Fill out your copyright notice in the Description page of Project Settings.


#include "GA_BasicAttack.h"

#include "ABasicAttackProjectile.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "MomentTriggerGameplayTags.h"

UE_DEFINE_GAMEPLAY_TAG(TAG_Event_BasicAttack_Hit, "Event.BasicAttack.Hit")

void UGA_BasicAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                      const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                      const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}
	UAbilityTask_PlayMontageAndWait* MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, AttackMontage);
	MontageTask->OnCompleted.AddDynamic(this, &UGA_BasicAttack::OnMontageComplated);
	MontageTask->OnInterrupted.AddDynamic(this, &UGA_BasicAttack::OnMontageComplated);
	MontageTask->OnCancelled.AddDynamic(this, &UGA_BasicAttack::OnMontageComplated);
	MontageTask->ReadyForActivation();
	
	UAbilityTask_WaitGameplayEvent* WaitEventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, TAG_Event_BasicAttack_Hit, nullptr, true);
	WaitEventTask->EventReceived.AddDynamic(this, &UGA_BasicAttack::OnHitEventReceived);
	WaitEventTask->ReadyForActivation();
	
}
void UGA_BasicAttack::OnHitEventReceived(FGameplayEventData Payload)
{
	AActor* Avatar = CurrentActorInfo->AvatarActor.Get();
	if (!Avatar)
	{
		return;
	}
	const FVector SpawnLocation = Avatar->GetActorLocation() + Avatar->GetActorForwardVector() * 100.0f;
	const FRotator SpawnRotation = Avatar->GetActorRotation();
	
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = Avatar;
	SpawnParams.Instigator = Cast<APawn>(Avatar);
	
	Avatar->GetWorld()->SpawnActor<AABasicAttackProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, SpawnParams);
	
}

void UGA_BasicAttack::OnMontageComplated()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

