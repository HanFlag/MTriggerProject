
#include "STTask_CircleStrafe.h"
#include "AIController.h"
#include "StateTreeExecutionContext.h"
#include "Navigation/PathFollowingComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

FSTTask_CircleStrafe::FSTTask_CircleStrafe()
{
}

const UStruct* FSTTask_CircleStrafe::GetInstanceDataType() const
{
	return FInstanceDataType::StaticStruct();
}

EStateTreeRunStatus FSTTask_CircleStrafe::EnterState(FStateTreeExecutionContext& Context,
	const FStateTreeTransitionResult& Transition) const
{
	FInstanceDataType& Data = Context.GetInstanceData(*this);
	if (!Data.Target || !Data.AIController || !Data.AIController->GetPawn())
	{
		return EStateTreeRunStatus::Failed;
	}
	
	// 방향은 랜덤 RandBool, 시간은 Min Max 랜덤
	Data.Dir = FMath::RandBool() ? 1.0f : -1.0f;
	Data.Duration = FMath::RandRange(Data.MinDuration, Data.MaxDuration);
	Data.Elapsed = 0.0f;
	Data.RepathTimer = 0.0f;
	ACharacter* Character = Cast<ACharacter>(Data.AIController->GetPawn());
	// 현재 원래 속도 저장
	if (Character && Character->GetCharacterMovement())
	{
		Data.SaveDefaultSpeed = Character->GetCharacterMovement()->MaxWalkSpeed;
		Data.TargetSpeed = FMath::FRandRange(Data.MinStrafeSpeed, Data.MaxStrafeSpeed);
		Data.SpeedTimer = FMath::FRandRange(1.0, 2.0);
		Character->GetCharacterMovement()->MaxWalkSpeed = Data.TargetSpeed;
		Data.SaveMaxAcceleration = Character->GetCharacterMovement()->MaxAcceleration;
		Data.SaveBrakingDecelerationWalking = Character->GetCharacterMovement()->BrakingDecelerationWalking;
		Character->GetCharacterMovement()->BrakingDecelerationWalking = Data.StrafeBraking;
		Character->GetCharacterMovement()->MaxAcceleration = Data.StrafeAcceleration;
	}
	
	return EStateTreeRunStatus::Running;
}

EStateTreeRunStatus FSTTask_CircleStrafe::Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const
{
	FInstanceDataType& Data = Context.GetInstanceData(*this);
	if (!Data.Target || !Data.AIController || !Data.AIController->GetPawn())
	{
		return EStateTreeRunStatus::Failed;
	}
	Data.Elapsed += DeltaTime;
	Data.RepathTimer -= DeltaTime;
	Data.SpeedTimer -= DeltaTime;
	if (Data.Elapsed >= Data.Duration)
	{
		return EStateTreeRunStatus::Succeeded;
	}
	// RepathTimer 가 0 이하 일때 목표 거리를 다시 계산하여 해당 지점으로 이동
	// 다시 Timer 새로고침
	if (Data.RepathTimer <= 0.0f)
	{
		FVector TargetLocation = Data.Target->GetActorLocation();
		FVector BossLocation = Data.AIController->GetPawn()->GetActorLocation();
		FVector Direction = (BossLocation - TargetLocation).GetSafeNormal2D();
		FVector Rotate = Direction.RotateAngleAxis(Data.Dir * Data.LeadAngle, FVector::UpVector);
		EPathFollowingRequestResult::Type Result = Data.AIController->MoveToLocation(TargetLocation + Rotate *Data.Radius, Data.AcceptanceRadius, true, true, true, true);
		Data.RepathTimer = Data.RepathInterval;
		if (EPathFollowingRequestResult::Failed == Result)
		{
			Data.Dir = -Data.Dir;
		}
	}
	ACharacter* Character = Cast<ACharacter>(Data.AIController->GetPawn());
	// 속도를 보간하여 Strafe 속도에 맞게 min~max 속도 보간 매 틱 속도 호출
	if (Character && Character->GetCharacterMovement())
	{
		if (Data.SpeedTimer <= 0.0f)
		{
			Data.TargetSpeed = FMath::FRandRange(Data.MinStrafeSpeed,Data.MaxStrafeSpeed);
			Data.SpeedTimer = FMath::FRandRange(1.0,2.0);
		}
		Character->GetCharacterMovement()->MaxWalkSpeed = FMath::FInterpTo(Character->GetCharacterMovement()->MaxWalkSpeed, Data.TargetSpeed, DeltaTime, 2.0f);
	}
	
	
	return EStateTreeRunStatus::Running;
}

void FSTTask_CircleStrafe::ExitState(FStateTreeExecutionContext& Context,
                                     const FStateTreeTransitionResult& Transition) const
{
	FInstanceDataType& Data = Context.GetInstanceData(*this);
	if (!Data.AIController)
	{
		return;
	}
	ACharacter* Character = Cast<ACharacter>(Data.AIController->GetPawn());
	if (Character && Character->GetCharacterMovement())
	{
		Character->GetCharacterMovement()->MaxWalkSpeed = Data.SaveDefaultSpeed;
		Character->GetCharacterMovement()->MaxAcceleration = Data.SaveMaxAcceleration;
		Character->GetCharacterMovement()->BrakingDecelerationWalking = Data.SaveBrakingDecelerationWalking;
	}
	
	Data.AIController->StopMovement();
}
