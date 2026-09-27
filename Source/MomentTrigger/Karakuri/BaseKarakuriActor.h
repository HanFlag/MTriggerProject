// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "BaseKarakuriActor.generated.h"

UCLASS()
class MOMENTTRIGGER_API ABaseKarakuriActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseKarakuriActor();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SceneComp")
	USceneComponent* SceneComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CollisionComp")
	UBoxComponent* CollisionComp;


	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	//캡슐화 목적으로 Getter, Setter 의 현장 ********* 중요 !
	void SetClusterInfo(const FVector& Origin, const FRotator& Rotation, int32 Index);
	FVector GetClusterOrigin() const {return ClusterOrigin;}
	FRotator GetClusterRotation() const {return ClusterRotation;}
	int32 GetClusterIndex() const {return ClusterIndex;}
	
	UPROPERTY(EditDefaultsOnly, Category = "Karakuri")
	float MaxHP = 30.0f;
	UPROPERTY(EditDefaultsOnly, Category = "Karakuri")
	float CounterDamage = 0.0f;
	
	//중복 파괴 방지
	bool bIsDestroyed = false;
	
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	
	UFUNCTION(BlueprintImplementableEvent, Category = "Karakuri")
	// 파괴 연출용 블프 훅
	void OnKarakuriDestroyed();
protected:
	FVector ClusterOrigin;
	FRotator ClusterRotation;
	int32 ClusterIndex;
	float CurrentHP;
	
	
};
