// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAnimInstance.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "Combat/MomentTriggerGameplayTags.h"
#include "Kismet/GameplayStatics.h"

void UEnemyAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	APawn* Pawn = TryGetPawnOwner();
	if (Pawn)
	{
		UAbilitySystemComponent* PawnASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Pawn);
		if (PawnASC)
		{
			GameplayTagPropertyMap.Initialize(this, PawnASC);
			OwnerASC = PawnASC;
		}
	}
}

void UEnemyAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	APawn* Pawn = TryGetPawnOwner();
	if (!Pawn)
	{
		return;
	}
	Speed = FVector::DotProduct(Pawn->GetActorForwardVector(), Pawn->GetVelocity());
	bIsChargeAiming = OwnerASC && OwnerASC->HasMatchingGameplayTag(TAG_State_Charge_Aiming);
	if (bIsChargeAiming)
	{
		APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
		if (!PlayerPawn)
		{
			return;
		}
		LookAtLocation = PlayerPawn->GetActorLocation();
		FVector LookAtDirection = LookAtLocation - Pawn->GetActorLocation();
		LookAtDirection.Z = 0.0f;
		if (LookAtDirection.IsNearlyZero())
		{
			return;
		}
		const float TargetYaw = LookAtDirection.Rotation().Yaw;
		AimYawOffset = FMath::FindDeltaAngleDegrees(Pawn->GetActorRotation().Yaw, TargetYaw);
	}
	else
	{
		AimYawOffset = 0.0f;
	}
}
#if WITH_EDITOR
#include "Misc/DataValidation.h"
EDataValidationResult UEnemyAnimInstance::IsDataValid(class FDataValidationContext& Context) const
{
	
	return CombineDataValidationResults(Super::IsDataValid(Context), GameplayTagPropertyMap.IsDataValid(this, Context));
}
#endif