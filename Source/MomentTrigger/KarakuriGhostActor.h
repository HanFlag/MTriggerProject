// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "KarakuriGhostActor.generated.h"

class UStaticMeshComponent;

/**
 * 카라쿠리 배치 위치를 보여주는 프리뷰(고스트) 액터. 실제 배치되는 오브젝트가 아니라 미리보기용.
 */
UCLASS()
class MOMENTTRIGGER_API AKarakuriGhostActor : public AActor
{
	GENERATED_BODY()

public:
	AKarakuriGhostActor();

	UPROPERTY(VisibleAnywhere, Category = "Karakuri")
	UStaticMeshComponent* PreviewMesh;

	void SetGhostVisible(bool bVisible);
	void UpdateGhostTransform(const FVector& NewLocation, const FRotator& NewRotation);
};
