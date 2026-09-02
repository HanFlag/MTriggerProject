// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "MomentTriggerPlayerController.generated.h"


class UUserWidget;
/**
 * 
 */
UCLASS()
class MOMENTTRIGGER_API AMomentTriggerPlayerController : public APlayerController
{
	GENERATED_BODY()
	public:
	AMomentTriggerPlayerController();
	
protected:
	/** Gameplay initialization */
	virtual void BeginPlay() override;

	/** Input mapping context setup */
	virtual void SetupInputComponent() override;
	
	virtual void Tick(float DeltaTime) override;
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	class UInputMappingContext* DefaultInputMappingContext;
		
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* MoveAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* MouseLock;
	
	
	void Move(const FInputActionValue& Value);
	
	void MouseLockTrigger();
	void MouseLockComplated();

	
public:
	bool bIsMouseLockPressed = false;
};
