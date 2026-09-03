// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "MomentTriggerCharacter.generated.h"

struct FInputActionValue;
class UInputAction;

UCLASS()
class MOMENTTRIGGER_API AMomentTriggerCharacter : public ACharacter
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
	
	void SetSprint(bool bEnable);

	void SetMouseLookState(bool bIsMouseLooking);
	void RotateToTargetLocation(const FVector& TargetLocation);
	
private:
	bool bIsDecelerationActive = false;
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float RotationInterpSpeed = 12.0f;
};
