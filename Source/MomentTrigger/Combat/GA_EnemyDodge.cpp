// Fill out your copyright notice in the Description page of Project Settings.


#include "GA_EnemyDodge.h"

#include "Abilities/Tasks/AbilityTask_ApplyRootMotionMoveToForce.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "GameFramework/Character.h"
#include "NavigationSystem.h"

void UGA_EnemyDodge::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	if (!DodgeRightMontage ||!DodgeLeftMontage ||!DodgeBackMontage)
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
	FVector Feet = Avatar->GetNavAgentLocation();
	FVector BackPoint = Feet - Avatar->GetActorForwardVector() * DodgeDistance;
	FVector LeftPoint = Feet - Avatar->GetActorRightVector() * DodgeDistance;
	FVector RightPoint = Feet + Avatar->GetActorRightVector() * DodgeDistance;
	FVector ChosenFeet = FVector::ZeroVector;
	UAnimMontage* ChosenMontage = nullptr;
	// 닷지 , 좌우 Evade 결정 코드
	if (IsDodgePathClear(Feet, BackPoint))
	{
		ChosenFeet = BackPoint;
		ChosenMontage = DodgeBackMontage;
	}
	else
	{
		bool bLeftFirst = FMath::RandBool();
		FVector FirstPoint = bLeftFirst ? LeftPoint : RightPoint;
		UAnimMontage* FirstMontage = bLeftFirst ? DodgeLeftMontage : DodgeRightMontage;
		FVector SecondPoint = bLeftFirst ? RightPoint : LeftPoint;
		UAnimMontage* SecondMontage = bLeftFirst ? DodgeRightMontage : DodgeLeftMontage;
		if (IsDodgePathClear(Feet, FirstPoint))
		{
			ChosenFeet = FirstPoint;
			ChosenMontage = FirstMontage;
		}
		else if (IsDodgePathClear(Feet, SecondPoint))
		{
			ChosenFeet = SecondPoint;
			ChosenMontage = SecondMontage;
		}
	}
	if (!ChosenMontage)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}
	if (!CommitAbility( Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	UAbilityTask_PlayMontageAndWait* ChosenMontageTask = nullptr;
	ChosenMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, ChosenMontage);
	if (!ChosenMontageTask)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}
	
	ChosenMontageTask->OnCancelled.AddDynamic(this, &UGA_EnemyDodge::OnDodgeCancelled);
	ChosenMontageTask->OnCompleted.AddDynamic(this, &UGA_EnemyDodge::OnDodgeFinished);
	ChosenMontageTask->OnInterrupted.AddDynamic(this, &UGA_EnemyDodge::OnDodgeCancelled);
	ChosenMontageTask->OnBlendOut.AddDynamic(this, &UGA_EnemyDodge::OnDodgeFinished);
	
	FVector TargetPoint = ChosenFeet + (Avatar->GetActorLocation() - Feet);
	UAbilityTask_ApplyRootMotionMoveToForce* DodgeMoveTask = UAbilityTask_ApplyRootMotionMoveToForce::ApplyRootMotionMoveToForce(this, NAME_None, TargetPoint, MoveDuration, false, MOVE_Walking, false, nullptr, ERootMotionFinishVelocityMode::SetVelocity, FVector::ZeroVector, 0.0f);
	if (!DodgeMoveTask)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}
	DodgeMoveTask->ReadyForActivation();
	ChosenMontageTask->ReadyForActivation();
}

void UGA_EnemyDodge::OnDodgeFinished()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UGA_EnemyDodge::OnDodgeCancelled()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}
// 헬퍼 함수 Start→End 경로가 내비메시 위로 열려 있는지 (NavigationRaycast 결과를 뒤집어 반환)
bool UGA_EnemyDodge::IsDodgePathClear(const FVector& Start, const FVector& End) const
{
	FVector HitLocation;
	
	return !UNavigationSystemV1::NavigationRaycast(GetWorld(), Start,End,HitLocation);
}
