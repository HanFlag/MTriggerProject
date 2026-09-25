// Fill out your copyright notice in the Description page of Project Settings.


#include "GA_EnemyBasicAttack.h"

#include "AbilitySystemGlobals.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "AbilityTask_PlayAnimAndWait.h"
#include "Combat/GA_BasicAttack.h"
#include "Enemy/EnemyCharacter.h"
#include "Kismet/KismetStringLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

void UGA_EnemyBasicAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                           const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                           const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}
	AEnemyCharacter* Avatar = Cast<AEnemyCharacter>(ActorInfo->AvatarActor.Get());
	if (!Avatar || AttackMontages.Num() == 0)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		 return;
	}
	Avatar->bIsAttacking = true;
	UAnimMontage* MontageToPlay = AttackMontages[FMath::RandRange(0, AttackMontages.Num()-1)];
	UAbilityTask_PlayMontageAndWait* MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, MontageToPlay);
	
	MontageTask->OnCompleted.AddDynamic(this, &UGA_EnemyBasicAttack::OnMontageCompleted);
	MontageTask->OnInterrupted.AddDynamic(this, &UGA_EnemyBasicAttack::OnMontageCompleted);
	MontageTask->OnCancelled.AddDynamic(this, &UGA_EnemyBasicAttack::OnMontageCompleted);
	MontageTask->ReadyForActivation();
	
	UAbilityTask_WaitGameplayEvent* WaitEventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, TAG_Event_BasicAttack_Hit, nullptr, true);
	WaitEventTask->EventReceived.AddDynamic(this, &UGA_EnemyBasicAttack::OnHitEventReceived);
	WaitEventTask->ReadyForActivation();
}

void UGA_EnemyBasicAttack::OnHitEventReceived(FGameplayEventData Payload)
{
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	TArray<AActor*> OutActors;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));
	AActor* Avatar = CurrentActorInfo->AvatarActor.Get();
	//전방 범위 액터 찾기
	UKismetSystemLibrary::SphereOverlapActors(this, Avatar->GetActorLocation() + Avatar->GetActorForwardVector() * AttackRange,AttackRange,ObjectTypes,nullptr, {Avatar},OutActors);
	UAbilitySystemComponent* EnemyASC = Cast<AEnemyCharacter>(Avatar)->AbilitySystemComp;
	if (!EnemyASC)
	{
		return;
	}
	for (AActor* HitActor : OutActors)
	{
		UAbilitySystemComponent* TargetASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(HitActor);
		if (!TargetASC)
		{
			continue;
		}
		
		FGameplayEffectContextHandle ContextHandle = EnemyASC->MakeEffectContext();
		FGameplayEffectSpecHandle SpecHandle = EnemyASC->MakeOutgoingSpec(DamageEffectClass, 1.0f, ContextHandle);
		if (!SpecHandle.IsValid())
		{
			UE_LOG(LogTemp, Warning, TEXT("DamageEffectClass is not valid"));
		}
		else
		{
			SpecHandle.Data->SetSetByCallerMagnitude(TAG_Data_Damage, -AttackDamage);
			EnemyASC->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), TargetASC);
		}
	}
	
	
}

void UGA_EnemyBasicAttack::OnMontageCompleted()
{
	AEnemyCharacter* Avatar = Cast<AEnemyCharacter>(CurrentActorInfo->AvatarActor.Get());
	if (!Avatar)
	{
		return;
	}
	Avatar->bIsAttacking = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
