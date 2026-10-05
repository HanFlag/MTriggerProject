// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Tasks/StateTreeAITask.h"
#include "STTask_CircleStrafe.generated.h"
class AAIController;


USTRUCT()
struct FSTTask_CircleStrafeInstanceData
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category = "Context")
	TObjectPtr<AAIController> AIController = nullptr;
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<AActor> Target = nullptr;
	UPROPERTY(EditAnywhere, Category = "Parameter")
	float Radius = 500.0f;
	UPROPERTY(EditAnywhere, Category = "Parameter")
	float LeadAngle = 40.0f;
	UPROPERTY(EditAnywhere, Category = "Parameter")
	float MinDuration = 2.0f;
	UPROPERTY(EditAnywhere, Category = "Parameter")
	float MaxDuration = 4.0f;
	UPROPERTY(EditAnywhere, Category = "Parameter")
	float RepathInterval = 0.25f;
	//허용반경
	UPROPERTY(EditAnywhere, Category = "Parameter")
	float AcceptanceRadius = 50.0f;
	UPROPERTY(EditAnywhere, Category = "Parameter")
	float MinStrafeSpeed = 60.0f;
	UPROPERTY(EditAnywhere, Category = "Parameter")
	float MaxStrafeSpeed = 150.0f;
	UPROPERTY(EditAnywhere, Category = "Parameter")
	float StrafeAcceleration = 300.0f;
	UPROPERTY(EditAnywhere, Category = "Parameter")
	float StrafeBraking = 250.0f;
	
	
	
	UPROPERTY()
	float Dir = 0.0f;
	UPROPERTY()
	float Duration = 0.0f;
	UPROPERTY()
	float Elapsed = 0.0f;
	UPROPERTY()
	float RepathTimer = 0.0f;
	UPROPERTY()
	float TargetSpeed = 0.0f;
	UPROPERTY()
	float SpeedTimer = 0.0f;
	UPROPERTY()
	float SaveDefaultSpeed = 0.0f;
	UPROPERTY()
	float SaveMaxAcceleration = 0.0f;
	UPROPERTY()
	float SaveBrakingDecelerationWalking = 0.0f;
	
};

USTRUCT(meta = (DisplayName = "Circle Strafe", Category = "AI|Action"))
struct FSTTask_CircleStrafe : public FStateTreeAITaskBase
{
	GENERATED_BODY()
	
public:
	FSTTask_CircleStrafe();
	using FInstanceDataType = FSTTask_CircleStrafeInstanceData;
	virtual const UStruct* GetInstanceDataType() const override;
	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;
	virtual EStateTreeRunStatus Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const override;
	virtual void ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;
	
};