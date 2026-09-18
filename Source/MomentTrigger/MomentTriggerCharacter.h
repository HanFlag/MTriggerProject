// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "AbilitySystemComponent.h"
#include "MomentTriggerCharacter.generated.h"

struct FInputActionValue;
class UInputAction;

UCLASS()
class MOMENTTRIGGER_API AMomentTriggerCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMomentTriggerCharacter();
	
	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArmComp;
	
	UPROPERTY(VisibleAnywhere)
	UCameraComponent* CameraComp;
	


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// 컨트롤러에 의해 소환된 적 AI에 적용되는것 시스템 구성 요소 초기화 전용
	virtual void PossessedBy(AController* NewController) override;
	//플레이어가 복제되는 시점 능력 시스템 구성 요소 초기화 전용
	virtual void OnRep_PlayerState() override;
	
	
	
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float JogSpeed = 400.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float SprintSpeed = 600.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float SpeedInterp = 3.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* TestAbilityAction;
	
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* AttackAction;

	
	// 어빌리티 콤프 정의
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AbilitySystem")
	UAbilitySystemComponent* AbilitySystemComp;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AbilitySystem")
	TSubclassOf<UGameplayAbility> TestAbilityClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AbilitySystem")
	TSubclassOf<UGameplayAbility> BasicAttackClass;
	
	void SetSprint(bool bEnable);

	void SetMouseLookState(bool bIsMouseLooking);
	void RotateToTargetLocation(const FVector& TargetLocation);
	void TestActivateAbility();
	void AttackInput();
	
	virtual  UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	
 	protected:
	// 어빌리티 시스템 복제 모드가 최소한이면 AI에게 적합 멀티플레이에 적합한 캐릭터의 복제 모드는 Mixed가 적합
	// 따라서 블루프린트 내에서 수정할 수 있게 UPROPERTY를 설정
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AbilitySystem")
	EGameplayEffectReplicationMode AscReplicationMode = EGameplayEffectReplicationMode::Mixed;
	
private:
	bool bIsDecelerationActive = false;
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float RotationInterpSpeed = 12.0f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AbilitySystem")
	class UMomentTriggerAttributeSet* AttributeSet;
	

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Karakuri")
	class UKarakuriPlacementComponent* KarakuriPlacementComp;
};
