// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"
#include "AbilitySystemComponent.h"
#include "AIController.h"
#include "BrainComponent.h"
#include "Combat/MomentTriggerAttributeSet.h"
#include "Components/CapsuleComponent.h"
#include "Abilities/GameplayAbility.h"

// Sets default values
AEnemyCharacter::AEnemyCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	AbilitySystemComp = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComp->SetIsReplicated(true);
	AttributeSet = CreateDefaultSubobject<UMomentTriggerAttributeSet>(TEXT("AttributeSet"));
}

// Called when the game starts or when spawned
void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	if (AttributeSet)
	{
		AbilitySystemComp->InitAbilityActorInfo(this,this);
		AttributeSet->OnHealthDepleted.AddUObject(this, &AEnemyCharacter::HandleDeath);
	}
	if (BasicAttackClass)
	{
		AbilitySystemComp->GiveAbility(FGameplayAbilitySpec(BasicAttackClass, 1, INDEX_NONE, this));
	}
	for (TSubclassOf<UGameplayAbility> AbilityClass : StartupAbilities)
	{
		if (AbilityClass)
		{
			AbilitySystemComp->GiveAbility(FGameplayAbilitySpec(AbilityClass, 1, INDEX_NONE, this));
		}
	}
}

UAbilitySystemComponent* AEnemyCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComp;
}



// Called to bind functionality to input
void AEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AEnemyCharacter::HandleDeath(AActor* DeadActor)
{
	if (bIsDead)
	{
		return;
	}
	bIsDead = true;
	if (AAIController* AICon = Cast<AAIController>(GetController()))
	{
		if (AICon->BrainComponent)
		{
			AICon->BrainComponent->StopLogic(TEXT("Died"));
		}
	}
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	OnDeath();
	SetLifeSpan(3.0f);
}
