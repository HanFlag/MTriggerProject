// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "EnemyCharacter.generated.h"

class UAbilitySystemComponent;
class UMomentTriggerAttributeSet;
class UGameplayEffect;
class UGameplayAbility;


UCLASS()
class MOMENTTRIGGER_API AEnemyCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	
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
	//공격 중인지?
	bool bIsAttacking = false;
	//돌진 중 문을 쳤는지?
	bool bChargeHitPendding = false;
	//돌진 데미지
	UPROPERTY(EditDefaultsOnly, Category = "Enemy")
	float ChargeDamage = 30.0f;
	
	float DefaultWalkSpeed = 0.0f;

	
	bool bIsDead = false;
	
	void HandleDeath(AActor* DeadActor);
	UFUNCTION(BlueprintImplementableEvent, Category = "Enemy")
	void OnDeath();
	
	
	
	UPROPERTY()
	UAbilitySystemComponent* AbilitySystemComp;
	UPROPERTY()
	UMomentTriggerAttributeSet* AttributeSet;
	UPROPERTY(EditDefaultsOnly, Category = "Enemy")
	TSubclassOf<UGameplayEffect> DamageEffectClass;
	UFUNCTION()
	void OnHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit);
	
	void EndChargeEarly();
	// 소형 적은 돌진이 막히도록 설정하고 대형 적은 플레이어에게 돌진 데미지를 주는 동시에 돌진 지속하는 패턴설정용
	UPROPERTY(EditDefaultsOnly, Category = "Enemy")
	bool bStopChargeOnHitPlayer = true;
	
	UPROPERTY(EditDefaultsOnly, Category= "Enemy")
	TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;
	UPROPERTY(EditDefaultsOnly, Category= "Enemy")
	TSubclassOf<UGameplayAbility> BasicAttackClass;
	
	
	

};
