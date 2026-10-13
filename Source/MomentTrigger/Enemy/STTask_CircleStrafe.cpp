
#include "STTask_CircleStrafe.h"
#include "AIController.h"
#include "StateTreeExecutionContext.h"
#include "NavigationSystem.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

// Start→End 경로가 내비메시 위로 열려 있는지 (NavigationRaycast 결과를 뒤집어 반환)
static bool IsStrafePathClear(UWorld* World, const FVector& Start, const FVector& End)
{
	FVector HitLocation;
	return !UNavigationSystemV1::NavigationRaycast(World, Start, End, HitLocation);
}

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
	// 이전 상태에서 걸어 둔 MoveTo가 남아 있으면 이동 입력과 싸우므로 정리
	Data.AIController->StopMovement();
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
	ACharacter* Character = Cast<ACharacter>(Data.AIController->GetPawn());
	if (!Character)
	{
		return EStateTreeRunStatus::Failed;
	}
	// 매 틱 앞서가는 목표점 쪽으로 이동 입력만 넣는다 (MoveTo 재요청 시 생기던 끊김 제거)
	FVector TargetLocation = Data.Target->GetActorLocation();
	FVector BossLocation = Character->GetActorLocation();
	FVector Direction = (BossLocation - TargetLocation).GetSafeNormal2D();
	FVector Rotate = Direction.RotateAngleAxis(Data.Dir * Data.LeadAngle, FVector::UpVector);
	FVector Goal = TargetLocation + Rotate * Data.Radius;
	FVector MoveDir = (Goal - BossLocation).GetSafeNormal2D();
	Character->AddMovementInput(MoveDir, 1.0f);

	// RepathInterval 마다 진행 방향 앞쪽이 막혔는지 검사, 막혔으면 반대로 돈다
	if (Data.RepathTimer <= 0.0f)
	{
		FVector Feet = Character->GetNavAgentLocation();
		FVector Ahead = Feet + MoveDir * Data.LookAheadDistance;
		if (!IsStrafePathClear(Character->GetWorld(), Feet, Ahead))
		{
			Data.Dir = -Data.Dir;
		}
		Data.RepathTimer = Data.RepathInterval;
	}
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
