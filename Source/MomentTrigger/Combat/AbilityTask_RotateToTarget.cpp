// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityTask_RotateToTarget.h"

UAbilityTask_RotateToTarget::UAbilityTask_RotateToTarget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bTickingTask = true;
}

UAbilityTask_RotateToTarget* UAbilityTask_RotateToTarget::RotateToTarget(UGameplayAbility* OwningAbility,
	AActor* InTarget, float InTurnRate)
{
	UAbilityTask_RotateToTarget* NewTask = NewAbilityTask<UAbilityTask_RotateToTarget>(OwningAbility);
	if (!NewTask)
	{
		return nullptr;
	}
	NewTask->Target = InTarget;
	NewTask->TurnRate = InTurnRate;
	return NewTask;
}

void UAbilityTask_RotateToTarget::TickTask(float DeltaTime)
{
	Super::TickTask(DeltaTime);
	AActor* Avatar = GetAvatarActor();
	if (!Avatar || !Target.IsValid())
	{
		return;
	}
	FVector TempLocation = Target->GetActorLocation() - Avatar->GetActorLocation();
	TempLocation.Z = 0.0f;
	if (TempLocation.IsNearlyZero())
	{
		return;
	}
	FVector SafeLocation = TempLocation.GetSafeNormal();
	FRotator Desired = SafeLocation.Rotation();
	FRotator NewRot = FMath::RInterpConstantTo(Avatar->GetActorRotation(), Desired, DeltaTime, TurnRate);
	Avatar->SetActorRotation(NewRot);
	
}
