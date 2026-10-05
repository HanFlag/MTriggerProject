// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "EnemyCharacter.generated.h"
class UMotionWarpingComponent;
class UAbilitySystemComponent;
class UMomentTriggerAttributeSet;
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

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	//공격 중인지?
	bool bIsAttacking = false;
	
	
	bool bIsDead = false;
	
	void HandleDeath(AActor* DeadActor);
	UFUNCTION(BlueprintImplementableEvent, Category = "Enemy")
	void OnDeath();
	
	
	
	UPROPERTY()
	UAbilitySystemComponent* AbilitySystemComp;
	UPROPERTY()
	UMomentTriggerAttributeSet* AttributeSet;
	UPROPERTY(EditDefaultsOnly, Category= "Enemy")
	TArray<TSubclassOf<UGameplayAbility>> 
	StartupAbilities;
	UPROPERTY(EditDefaultsOnly, Category= "Enemy")
	TSubclassOf<UGameplayAbility> BasicAttackClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation")
	TObjectPtr<UMotionWarpingComponent> MotionWarping;
	
	

};
