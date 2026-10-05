// Fill out your copyright notice in the Description page of Project Settings.

#include "GA_EnemyTurn.h"
#include "GameFramework/Character.h"
#include "MotionWarpingComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Kismet/GameplayStatics.h"

void UGA_EnemyTurn::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                    const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return ;
	}
	// 아바타(보스) 플레이어 폰의 캐스트 형태
	ACharacter* Avatar = Cast<ACharacter>(ActorInfo->AvatarActor.Get());
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (!Avatar || !PlayerPawn)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}
	// 타겟 Yaw 계산
	FVector ToPlayer = PlayerPawn->GetActorLocation() - Avatar->GetActorLocation();
	ToPlayer.Z = 0.0f;
	float DeltaYaw = FMath::FindDeltaAngleDegrees(Avatar->GetActorRotation().Yaw, ToPlayer.Rotation().Yaw);
	float AbsDelta = FMath::Abs(DeltaYaw);
	if (AbsDelta < MinTurnAngle)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}
	UAnimMontage* MontageToPlay = nullptr;
	if (AbsDelta >= Use180Angle)
	{
		MontageToPlay = (DeltaYaw > 0.0f) ? TurnR180Montage : TurnL180Montage;
	}
	else
	{
		MontageToPlay = (DeltaYaw > 0.0f) ? TurnR90Montage : TurnL90Montage;
	}
	if (!MontageToPlay)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}
	UMotionWarpingComponent* Warp = Avatar->FindComponentByClass<UMotionWarpingComponent>();
	// 보스 위치 얻고, 현재 플레이어 방향을 얻어냄
	if (Warp)
	{
	Warp->AddOrUpdateWarpTargetFromLocationAndRotation(WarpTargetName, Avatar->GetActorLocation(), FRotator(0.0f, ToPlayer.Rotation().Yaw, 0.0f));
	}


	UAbilityTask_PlayMontageAndWait* TurnTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this, NAME_None, MontageToPlay);
	if (!TurnTask)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}
	
	TurnTask->OnCancelled.AddDynamic(this, &UGA_EnemyTurn::OnTurnMontageFinished);
	TurnTask->OnInterrupted.AddDynamic(this, &UGA_EnemyTurn::OnTurnMontageFinished);
	TurnTask->OnCompleted.AddDynamic(this, &UGA_EnemyTurn::OnTurnMontageFinished);
	TurnTask->OnBlendOut.AddDynamic(this, &UGA_EnemyTurn::OnTurnMontageFinished);
	TurnTask->ReadyForActivation();

}

void UGA_EnemyTurn::OnTurnMontageFinished()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
