// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "AIController.h"
#include "BrainComponent.h"
#include "Combat/MomentTriggerAttributeSet.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Combat/MomentTriggerGameplayTags.h"
#include "Abilities/GameplayAbility.h"
#include "Engine/DamageEvents.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AEnemyCharacter::AEnemyCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
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
	DefaultWalkSpeed = GetCharacterMovement()->MaxWalkSpeed;
	OnActorHit.AddDynamic(this, &AEnemyCharacter::OnHit);
}

UAbilitySystemComponent* AEnemyCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComp;
}

// Called every frame
void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (ChargeCooldownRemaining > 0.0f)
	{
		ChargeCooldownRemaining -= DeltaTime;
	}

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

void AEnemyCharacter::OnHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit)
{
	if (!bIsCharging || !OtherActor)
	{
		return;
	}
	
	if (OtherActor && OtherActor->ActorHasTag(TEXT("KarakuriDoor")))
	{
		EndChargeEarly();
		OtherActor->TakeDamage(ChargeDamage, FDamageEvent(), GetController(), this);
		UE_LOG(LogTemp, Warning, TEXT("HitActor %s"), *OtherActor->GetName());
		UE_LOG(LogTemp, Warning, TEXT("HitActor %s"), *OtherActor->GetActorLabel());
	}
	else
	{
		UAbilitySystemComponent* PlayerASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(OtherActor);
		if (!PlayerASC)
		{
			return;
		}
		if (bStopChargeOnHitPlayer)
		{
			EndChargeEarly();	
		}
		
		//ContextHandle == 데미지를 누가 유발했는지
		FGameplayEffectContextHandle ContextHandle = AbilitySystemComp->MakeEffectContext();
		FGameplayEffectSpecHandle SpecHandle = AbilitySystemComp->MakeOutgoingSpec(DamageEffectClass, 1.0f,ContextHandle);
		if (!SpecHandle.IsValid())
		{
			UE_LOG(LogTemp, Warning, TEXT("DamageEffectClass is not valid"));
		}
		else
		{
			SpecHandle.Data->SetSetByCallerMagnitude(TAG_Data_Damage, -ChargeDamage);
			//호출 주체는 공격을 가한 쪽
			AbilitySystemComp->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), PlayerASC);
		}
		
	}
}

void AEnemyCharacter::EndChargeEarly()
{
	GetCharacterMovement()->MaxWalkSpeed = DefaultWalkSpeed;
	ChargeCooldownRemaining = ChargeCooldownDuration;
	ChargeElapsed = 0.0f;
	bChargeHitPendding = true;
	//bIsCharging 나중에 문 체력이 낮은상태로 돌진을 한다면 OnHit 시 돌진 데미지가 문 HP보다 더 높을경우 차징을 계속해야하는지 멈춰야하는 지 고려대상
	bIsCharging = false;
}
