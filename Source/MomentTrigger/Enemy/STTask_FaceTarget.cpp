
#include "STTask_FaceTarget.h"
#include "AIController.h"
#include "StateTreeExecutionContext.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "Combat/MomentTriggerGameplayTags.h"

FSTTask_FaceTarget::FSTTask_FaceTarget()
{
	bShouldCallTick = false;
	bConsideredForCompletion = false;
}
//  복사해서 쓰는 한 줄
const UStruct* FSTTask_FaceTarget::GetInstanceDataType() const
{
	return FInstanceDataType::StaticStruct();
}

EStateTreeRunStatus FSTTask_FaceTarget::EnterState(FStateTreeExecutionContext& Context,
	const FStateTreeTransitionResult& Transition) const
{
	FInstanceDataType& Data = Context.GetInstanceData(*this);
	if (!Data.Target || !Data.AIController)
	{
		return EStateTreeRunStatus::Failed;
	}
	Data.AIController->SetFocus(Data.Target);
	ACharacter* Character = Cast<ACharacter>(Data.AIController->GetPawn());
	if (Character)
	{
		//이동방향 끄고 컨트롤러 보는쪽으로 몸 돌리기 체크
		if (Character->GetCharacterMovement())
		{
		Character->GetCharacterMovement()->bOrientRotationToMovement = false;
		Character->GetCharacterMovement()->bUseControllerDesiredRotation = true;
			
		}
		
	}
	UAbilitySystemComponent* PawnASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Character);
	if (PawnASC)
	{
		PawnASC->AddLooseGameplayTag(TAG_State_Boss_CombatMove);
	}
	return EStateTreeRunStatus::Running;
}

void FSTTask_FaceTarget::ExitState(FStateTreeExecutionContext& Context,
	const FStateTreeTransitionResult& Transition) const
{
	FInstanceDataType& Data = Context.GetInstanceData(*this);
	if (!Data.AIController)
	{
		return;
	}
	Data.AIController->ClearFocus(EAIFocusPriority::Gameplay);
	ACharacter* Character = Cast<ACharacter>(Data.AIController->GetPawn());
	if (Character)
	{
		//이동방향 끄고 컨트롤러 보는쪽으로 몸 돌리기 체크
		if (Character->GetCharacterMovement())
		{
		Character->GetCharacterMovement()->bOrientRotationToMovement = true;
		Character->GetCharacterMovement()->bUseControllerDesiredRotation = false;
		}
		
	}
	UAbilitySystemComponent* PawnASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Character);
	if (PawnASC)
	{
		PawnASC->RemoveLooseGameplayTag(TAG_State_Boss_CombatMove);
	}
}
