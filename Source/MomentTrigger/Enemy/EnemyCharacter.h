// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyCharacter.generated.h"

UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	Idle,
	Chase,
	Charge
};

UCLASS()
class MOMENTTRIGGER_API AEnemyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	EEnemyState CurrentState = EEnemyState::Idle;
	
	//감지거리
	UPROPERTY(EditDefaultsOnly, Category = "Enemy")
	float DetectionRadius = 800.0f;
	// 돌진 속도
	UPROPERTY(EditDefaultsOnly, Category = "Enemy")
	float ChargeSpeed = 1500.0f;
	// 쿨다운 시스템인데 GAS 로 구현전 간단한 float 형 구현
	UPROPERTY(EditDefaultsOnly, Category = "Enemy")
	float ChargeCooldownDuration = 30.0f;
	float ChargeCooldownRemaining = 0.0f;
	// 돌진 시 준비자세 시간
	UPROPERTY(EditDefaultsOnly, Category = "Enemy")
	float ChargeWindupDuration = 2.0f;
	float ChargeWindupElapsed = 0.0f;
	// 돌진 지속 시간
	UPROPERTY(EditDefaultsOnly, Category = "Enemy")
	float ChargeDuration = 1.0f;
	float ChargeElapsed = 0.0f;
	// 돌전 시작 시 고정 회전 방향
	FVector ChargeDirection;
	//돌진 중인지?
	bool bIsCharging = false;
	//돌진 중 문을 쳤는지?
	bool bDoorHitPending = false;
	
	
	float DefaultWalkSpeed = 0.0f;
	
	UFUNCTION()
	void OnHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit);

};
