// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"
#include "AbilitySystemComponent.h"
#include "AIController.h"
#include "AssetDefinitionAssetInfo.h"
#include "BrainComponent.h"
#include "Combat/MomentTriggerAttributeSet.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/CapsuleComponent.h"
#include "SWarningOrErrorBox.h"
#include "Kismet/GameplayStatics.h"
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
	if (!bIsCharging)
	{
		return;
	}

	if (OtherActor && OtherActor->ActorHasTag(TEXT("KarakuriDoor")))
	{
		GetCharacterMovement()->MaxWalkSpeed = DefaultWalkSpeed;
		ChargeCooldownRemaining = ChargeCooldownDuration;
		ChargeElapsed = 0.0f;
		bDoorHitPending = true;
		//bIsCharging 나중에 문 체력이 낮은상태로 돌진을 한다면 OnHit 시 돌진 데미지가 문 HP보다 더 높을경우 차징을 계속해야하는지 멈춰야하는 지 고려대상
		bIsCharging = false;
		OtherActor->Destroy();
		UE_LOG(LogTemp, Warning, TEXT("HitActor %s"), *OtherActor->GetName());
		UE_LOG(LogTemp, Warning, TEXT("HitActor %s"), *OtherActor->GetActorLabel());
	}
}
