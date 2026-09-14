// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseKarakuriActor.h"
#include "Components/ActorComponent.h"
#include "KarakuriPlacementComponent.generated.h"

class AKarakuriGhostActor;

/**
 * 카라쿠리 배치 프리뷰(고스트) 표시/이동을 담당. 실제 배치/레시피 로직은 다음 단계에서 추가 예정.
 */
UCLASS(ClassGroup=(Karakuri), meta=(BlueprintSpawnableComponent))
class MOMENTTRIGGER_API UKarakuriPlacementComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UKarakuriPlacementComponent();

	// 우클릭 시작 시 호출
	void BeginPlacementPreview();

	// 우클릭 유지 중 매 틱 호출 - Owner(캐릭터) 위치/방향 기준으로 고스트 위치 갱신
	void UpdatePlacementPreview();

	// 우클릭 뗄 때 호출
	void EndPlacementPreview();
	
	ABaseKarakuriActor* FindKarakuriAtLocation(const FVector& TargetLocation) const;

protected:
	virtual void BeginPlay() override;

	// 고스트로 스폰할 액터 클래스 (BP에서 메시 지정)
	UPROPERTY(EditDefaultsOnly, Category = "Karakuri")
	TSubclassOf<AKarakuriGhostActor> GhostActorClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Karakuri")
	TSubclassOf<ABaseKarakuriActor> KarakuriClass;

	UPROPERTY()
	AKarakuriGhostActor* GhostActor;

	UPROPERTY(EditDefaultsOnly, Category = "Karakuri")
	float GridCellSize = 100.0f;

	// HitResult의 액터가 "KarakuriGround" 태그를 가진 설치 가능 표면인지 판정
	bool IsBuildableSurface(const FHitResult& HitResult) const;
	
	// 토글 상태 기억 변수 나중에 추가 예정***
	//bool bFreePlacementOverride = false;

	// 회전을 8방향(45도 단위) 중 가장 가까운 값으로 스냅
	FRotator SnapRotationToCardinal(const FRotator& ToCursor) const;
	
	// 배치한 카라쿠리를 반경 내 가까운 카라쿠리를 확인하기
	//ABaseKarakuriActor* FindNearestKarakuriAnchor(const FVector& PlayerLocation) const;
	

	// 락 여부
	bool bIsSnapLocked = false;
	// 락 앵커 위치
	// FVector LockedAnchorLocation;
	// FRotator LockedAnchorRotation;
	// 카라쿠리 위치 확인
	FVector OriginLocation;
	FRotator OriginRotation;
	// 락 앵커 해제 반경
	UPROPERTY(EditDefaultsOnly, Category = "Karakuri")
	float LockedAnchorRadius = 200.0f;
	// 스폰 위치가 유효한지 확인
	bool bIsValidSpawnLocation = false;
	// 지금 몇번째 배치인지
	int32 PlacementCount = 0;
	
	

public:

	void SpawnKarakuriActor();
};
