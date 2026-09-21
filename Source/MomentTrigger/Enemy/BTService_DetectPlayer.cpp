// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_DetectPlayer.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

UBTService_DetectPlayer::UBTService_DetectPlayer()
{
	NodeName = TEXT("Detect Player"); 
	Interval = 0.2f;
}

void UBTService_DetectPlayer::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	
	
	if (AAIController* AiCon = OwnerComp.GetAIOwner())
	{
		APawn* ControlledPawn = AiCon->GetPawn();
		if (ControlledPawn)
		{
			APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
			if (!PlayerPawn)
			{
				return;
			}
			FVector PlayyerLocation = PlayerPawn->GetActorLocation();
			float PlayerDistance = FVector::Dist(ControlledPawn->GetActorLocation(), PlayyerLocation);
			if (PlayerDistance < DetectionRadius)
			{
				OwnerComp.GetBlackboardComponent()->SetValueAsObject(GetSelectedBlackboardKey(), PlayerPawn);
			}
			else
			{
				OwnerComp.GetBlackboardComponent()->ClearValue(GetSelectedBlackboardKey());
			}
			
		}
	}
}
