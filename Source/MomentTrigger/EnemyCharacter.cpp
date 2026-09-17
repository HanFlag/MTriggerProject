// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"

#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AEnemyCharacter::AEnemyCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	DefaultWalkSpeed = GetCharacterMovement()->MaxWalkSpeed;
	OnActorHit.AddDynamic(this, &AEnemyCharacter::OnHit);
}

// Called every frame
void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (ChargeCooldownRemaining > 0.0f)
	{
		ChargeCooldownRemaining -= DeltaTime;
	}

	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);
	if (!PlayerPawn)
	{
		return;
	}
	const float DistToPlayer = FVector::Dist(GetActorLocation(), PlayerPawn->GetActorLocation());

	switch (CurrentState)
	{
	case EEnemyState::Idle:
		if (DistToPlayer <= DetectionRadius)
		{
			CurrentState = EEnemyState::Chase;
		}
		break;
	case EEnemyState::Chase:
		if (DistToPlayer > DetectionRadius)
		{
			CurrentState = EEnemyState::Idle;
			break;
		}
		if (ChargeCooldownRemaining <= 0.0f)
		{
			//돌진 준비 시작
			CurrentState = EEnemyState::Charge;
			ChargeWindupElapsed = 0.0f;
		}
		else
		{
			//쿨다운중 추격
			const FVector ToPlayer =(PlayerPawn->GetActorLocation() - GetActorLocation()).GetSafeNormal();
			AddMovementInput(ToPlayer);
		}
		break;
	case EEnemyState::Charge:
		if (ChargeWindupElapsed < ChargeWindupDuration)
		{
			//준비 모션중 방향만 갱신
			ChargeWindupElapsed += DeltaTime;
			ChargeDirection = (PlayerPawn->GetActorLocation() - GetActorLocation()).GetSafeNormal();
		}
		else
		{
			//돌진
			GetCharacterMovement()->MaxWalkSpeed = ChargeSpeed;
			AddMovementInput(ChargeDirection);
			
			ChargeElapsed += DeltaTime;
			if (ChargeElapsed >= ChargeDuration)
			{
				GetCharacterMovement()->MaxWalkSpeed = DefaultWalkSpeed;
				ChargeCooldownRemaining = ChargeCooldownDuration;
				ChargeElapsed = 0.0f;
				CurrentState = EEnemyState::Chase;
			}
		}
		break;
	}
}

// Called to bind functionality to input
void AEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AEnemyCharacter::OnHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit)
{
	if (CurrentState != EEnemyState::Charge)
	{
		return;
	}
	if (OtherActor && OtherActor->ActorHasTag(TEXT("KarakuriDoor")))
	{
		GetCharacterMovement()->MaxWalkSpeed = DefaultWalkSpeed;
		ChargeCooldownRemaining = ChargeCooldownDuration;
		ChargeElapsed = 0.0f;
		CurrentState = EEnemyState::Chase;
		OtherActor->Destroy();
	}
}
