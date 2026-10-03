// Fill out your copyright notice in the Description page of Project Settings.


#include "STTask_TrackPlayer.h"
#include "StateTreeExecutionContext.h"
#include "AIController.h"
#include "Kismet/GameplayStatics.h"
#include "Core/MomentTriggerCharacter.h"

static void UpdateTrackPlayerData(FSTTask_TrackPlayerInstanceData& Data)
{
	Data.PlayerActor = nullptr;
	Data.DistanceToPlayer = 0.0f;
	Data.bPlayerAlive = false;
	
	if (!Data.AIController)
	{
		return;
	}
	APawn* BossPawn = Data.AIController->GetPawn();
	if (!BossPawn)
	{
		return;
	}
	APawn* Player = UGameplayStatics::GetPlayerPawn(BossPawn, 0);
	if (!Player)
	{
		return;
	}
	Data.DistanceToPlayer = FVector::Dist2D(BossPawn->GetActorLocation(), Player->GetActorLocation());
	Data.PlayerActor = Player;
	AMomentTriggerCharacter* PlayerCharacter = Cast<AMomentTriggerCharacter>(Player);
	if (PlayerCharacter && !PlayerCharacter->bIsDead)
	{
		Data.bPlayerAlive = true;
	}
	
}

const UStruct* FSTTask_TrackPlayer::GetInstanceDataType() const
{return FInstanceDataType::StaticStruct();}

EStateTreeRunStatus FSTTask_TrackPlayer::EnterState(FStateTreeExecutionContext& Context,
	const FStateTreeTransitionResult& Transition) const
{
	FInstanceDataType& Data = Context.GetInstanceData(*this);
	UpdateTrackPlayerData(Data);
	return EStateTreeRunStatus::Running;
	
	
}

EStateTreeRunStatus FSTTask_TrackPlayer::Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const
{
	FInstanceDataType& Data = Context.GetInstanceData(*this);
	UpdateTrackPlayerData(Data);
	return EStateTreeRunStatus::Running;
}
