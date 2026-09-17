// Fill out your copyright notice in the Description page of Project Settings.


#include "KarakuriPlacementComponent.h"

#include <rapidjson/document.h>
#include "EngineUtils.h"
#include "KarakuriGhostActor.h"
#include "VerseVM/VVMVerseEnum.h"


UKarakuriPlacementComponent::UKarakuriPlacementComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UKarakuriPlacementComponent::BeginPlay()
{
	Super::BeginPlay();

	if (GhostActorClass)
	{
		GhostActor = GetWorld()->SpawnActor<AKarakuriGhostActor>(GhostActorClass);
	}
}

void UKarakuriPlacementComponent::BeginPlacementPreview()
{
	if (GhostActor)
	{
		GhostActor->SetGhostVisible(true);
	}
}

void UKarakuriPlacementComponent::UpdatePlacementPreview()
{
	AActor* OwnerActor = GetOwner();
	if (!GhostActor || !OwnerActor)
	{
		return;
	}
	/*
	// 이번 틱에 V(스냅 중단)키가 눌렸으면 뒤집기 나중에 추가 예정***
	if (bSnapOverride)
	{
		//bFreePlacementOverride = !bFreePlacementOverride;
	}
	*/
	const float DistToOrigin = FVector::Dist(OwnerActor->GetActorLocation(),OriginLocation);
	bIsSnapLocked = (PlacementCount > 0) && (DistToOrigin < LockedAnchorRadius) ; //&&//!bFreePlacementOverride;나중에 추가 예정***
	
	FVector CandidateLocation;
	FRotator CandidateRotation;
	
	if (!bIsSnapLocked)
	{
		CandidateRotation = SnapRotationToCardinal(OwnerActor->GetActorRotation());
		const FVector Forward = OwnerActor->GetActorLocation() + CandidateRotation.Vector() * GridCellSize;
		//그리드 관련 업데이트틑 빼고, 월드 좌표로 투영하기로 함
		// FVector GridSnapped = FVector(FMath::GridSnap(Forward.X, GridCellSize), FMath::GridSnap(Forward.Y, GridCellSize), Forward.Z);
		
		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(OwnerActor);
		const FVector TraceStart = Forward + FVector(0.0f, 0.0f, 200.0f);
		const FVector TraceEnd = Forward - FVector(0.0f, 0.0f, 200.0f);
		FHitResult GroundHit;
		const bool bHitGround = GetWorld()->LineTraceSingleByChannel(GroundHit, TraceStart, TraceEnd, ECC_Visibility, QueryParams);
		if (!bHitGround || !IsBuildableSurface(GroundHit))
		{
			GhostActor->SetGhostVisible(false);
			bIsValidSpawnLocation = false;
			return;
		}
		CandidateLocation = GroundHit.ImpactPoint;
	}
	else
	{
		const int32 ColumnIndex = PlacementCount / 3;
		const int32 HeightIndex = PlacementCount % 3;
		const FVector RightAxis = OriginRotation.RotateVector(FVector::RightVector);
		UE_LOG(LogTemp, Warning, TEXT("ColumnIndex : %d, HeightIndex : %d"), ColumnIndex, HeightIndex);
		
		CandidateLocation = OriginLocation + (RightAxis * GridCellSize * ColumnIndex) + FVector(0.0f, 0.0f, GridCellSize * HeightIndex);
		CandidateRotation = OriginRotation;
	}
	bIsValidSpawnLocation = true;
	GhostActor->SetGhostVisible(true);
	GhostActor->UpdateGhostTransform(CandidateLocation, CandidateRotation);
	
	
	/*
	 // 커서 좌표 기준 카라쿠리 설치 코드
	*AActor* OwnerActor = GetOwner();
	if (!GhostActor || !OwnerActor)
	{
		return;
	}
	ABaseKarakuriActor* NearestKarakuri = FindNearestKarakuriAnchor(OwnerActor->GetActorLocation());
	// 토글 기능 눌렸는지 기억, SpawnKarakuriActor에서 처리함
	if (bSnapOverride)
	{
		bFreePlacementOverride = !bFreePlacementOverride;
	}
	bIsSnapLocked = (NearestKarakuri != nullptr) && !bFreePlacementOverride;
	if (bIsSnapLocked)
	{
		LockedAnchorLocation = NearestKarakuri->GetActorLocation();
		LockedAnchorRotation = NearestKarakuri->GetActorRotation();
	}
	
	const FRotator SnappedRotation = SnapRotationToCardinal(OwnerActor->GetActorRotation());
	if (!bIsSnapLocked)
	{
		// 배치될 방향과 위치 설정
		const FVector CandidateLocation = OwnerActor->GetActorLocation() + SnappedRotation.Vector() * GridCellSize;
		//배치 후 월드 좌표가 아닌 직접 그리드 좌표로 환산 후 적용
		FVector CandidateLocationGridSnap = FVector(FMath::GridSnap(CandidateLocation.X, GridCellSize),FMath::GridSnap(CandidateLocation.Y, GridCellSize),CandidateLocation.Z);
		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(OwnerActor);
		const FVector TraceStart = CandidateLocationGridSnap + FVector(0.0f, 0.0f, 200.0f);
		const FVector TraceEnd = CandidateLocationGridSnap - FVector(0.0f, 0.0f, 200.0f);

		FHitResult GroundHit;
		const bool bHitGround = GetWorld()->LineTraceSingleByChannel(GroundHit, TraceStart, TraceEnd, ECC_Visibility, QueryParams);

		if (!bHitGround || !IsBuildableSurface(GroundHit))
		{
			GhostActor->SetGhostVisible(false);
			bIsValidSpawnLocation = false;
			return;
		}
		bIsValidSpawnLocation = true;
		GhostActor->SetGhostVisible(true);
		GhostActor->UpdateGhostTransform(GroundHit.ImpactPoint, SnappedRotation);
	}
	if (bIsSnapLocked)
	{
		// 커서가 정확히 뭘 맞췄는지가 아니라, 앵커에서 커서 지점까지의 "방향"만 봄
		const FVector ToCursor = CursorHit.ImpactPoint - LockedAnchorLocation;
		const float FlatDist = FVector(ToCursor.X, ToCursor.Y, 0.0f).Size();

		// 커서가 앵커 바로 위/근처를 가리키면 위로, 아니면 그 방향으로 스냅
		const float RelativeYaw = ToCursor.Rotation().Yaw - LockedAnchorRotation.Yaw;
		const float SnappedRelativeYaw = FMath::RoundToFloat(RelativeYaw / 90.0f) * 90.0f;
		// const FRotator OffsetDirection = (FRotator(0.0f, SnappedRelativeYaw + LockedAnchorRotation.Yaw, 0.0f));
		const FVector OffsetVector = (FlatDist < GridCellSize * 0.5f) 
		? FVector::ZeroVector : 
		FRotator(0.0f, SnappedRelativeYaw + LockedAnchorRotation.Yaw, 0.0f).Vector();

		const FVector HorizontalCandidate = LockedAnchorLocation + OffsetVector * GridCellSize;

		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(OwnerActor);
		const FVector TraceStart = HorizontalCandidate + FVector(0.0f, 0.0f, 200.0f);
		const FVector TraceEnd = HorizontalCandidate - FVector(0.0f, 0.0f, 200.0f);

		FHitResult SideGroundHit;
		const bool bHitSideGround = GetWorld()->LineTraceSingleByChannel(SideGroundHit, TraceStart, TraceEnd, ECC_Visibility, QueryParams);

		if (!bHitSideGround || !IsBuildableSurface(SideGroundHit))
		{
			GhostActor->SetGhostVisible(false);
			bIsValidSpawnLocation = false;
			return;
		}

		bIsValidSpawnLocation = true;
		GhostActor->SetGhostVisible(true);
		GhostActor->UpdateGhostTransform(SideGroundHit.ImpactPoint, LockedAnchorRotation);
		}*/
	
	/*
	if (bIsSnapLocked)
	{
		FVector CandidateLocation;
		FRotator CandidateRotation = LockedAnchorRotation;
		if (CursorHit.ImpactNormal.Z > 0.7f)
		{
			CandidateLocation = FVector(LockedAnchorLocation.X, LockedAnchorLocation.Y, CursorHit.ImpactPoint.Z);
			CandidateRotation = LockedAnchorRotation;

		}
		else
		{
			const FRotator OffsetDirection = SnapRotationToCardinal(CursorHit.ImpactNormal.Rotation());
			const FVector HorizontalCandidate = LockedAnchorLocation + OffsetDirection.Vector() * GridCellSize;

			FCollisionQueryParams QueryParams;
			QueryParams.AddIgnoredActor(OwnerActor);
			const FVector TraceStart = HorizontalCandidate + FVector(0.0f, 0.0f, 200.0f);
			const FVector TraceEnd = HorizontalCandidate - FVector(0.0f, 0.0f, 200.0f);

			FHitResult SideGroundHit;
			const bool bHitSideGround = GetWorld()->LineTraceSingleByChannel(SideGroundHit, TraceStart, TraceEnd, ECC_Visibility, QueryParams);
			if (!bHitSideGround || !IsBuildableSurface(SideGroundHit))
			{
				GhostActor->SetGhostVisible(false);
				bIsValidSpawnLocation = false;
				return;
			}
			CandidateLocation = SideGroundHit.ImpactPoint;
		}

		bIsValidSpawnLocation = true;
		GhostActor->SetGhostVisible(true);
		GhostActor->UpdateGhostTransform(CandidateLocation, CandidateRotation);
	}
	*/

}

void UKarakuriPlacementComponent::EndPlacementPreview()
{
	if (GhostActor)
	{
		GhostActor->SetGhostVisible(false);
	}
}

ABaseKarakuriActor* UKarakuriPlacementComponent::FindKarakuriAtLocation(const FVector& TargetLocation) const
{
	//컴파일시 확정 값
	constexpr float Tolerance = 10.0f;
	for (TActorIterator<ABaseKarakuriActor> It(GetWorld()); It; ++It)
	{
		ABaseKarakuriActor* Karakuri = *It;
		if (FVector::DistSquared(Karakuri->GetActorLocation(), TargetLocation) < FMath::Square(Tolerance))
		{
			return Karakuri;
		}
	}
	return nullptr;
}

bool UKarakuriPlacementComponent::CheckDoorRecipe(TArray<ABaseKarakuriActor*>& OutFoundKarakuri) const
{
	// 값 초기화
	OutFoundKarakuri.Empty();
	const FVector RightAxis = OriginRotation.RotateVector(FVector::RightVector);
	
	for (int32 Column = 0; Column < 2; ++Column)
	{
		for (int32 Height = 0; Height < 3; ++Height)
		{
			const FVector CheckLocation = OriginLocation + (RightAxis * GridCellSize * Column) + FVector(0.0f, 0.0f, GridCellSize * Height);
			ABaseKarakuriActor* Found = FindKarakuriAtLocation(CheckLocation);
			if (!Found)
			{
				return false;
			}
			OutFoundKarakuri.Add(Found);
		}
	}
	return true;
}
// 문 레시피 확정 시 Spawn 함수에서 호출
void UKarakuriPlacementComponent::TryCompleteDoorRecipe()
{
	TArray<ABaseKarakuriActor*> FoundKarakuri;
	if (!CheckDoorRecipe(FoundKarakuri))
	{
		return;
	}
	const FVector RightAxis = OriginRotation.RotateVector(FVector::RightVector);
	const FVector DoorLocation = OriginLocation + (RightAxis * GridCellSize * 0.5f)+FVector(0.0f,0.0f,GridCellSize);
	for (ABaseKarakuriActor* Karakuri : FoundKarakuri)
	{
		//Complate시 삭제
		Karakuri->Destroy();
	}
	if (DoorRecipeClass)
	{
		//블루프린트 프로퍼티 설정된 액터 불러오기, 좌표, 회전 값
		AActor* BlockingDoor = GetWorld()->SpawnActor<AActor>(DoorRecipeClass, DoorLocation, OriginRotation);
		if (BlockingDoor)
		{
			BlockingDoor->Tags.Add(TEXT("KarakuriDoor"));
		}
	}
	// 클러스터 0 으로 초기화
	PlacementCount = 0;
}

bool UKarakuriPlacementComponent::IsBuildableSurface(const FHitResult& HitResult) const
{
	static const FName BuildableSurfaceTag(TEXT("KarakuriGround"));

	const AActor* HitActor = HitResult.GetActor();
	return HitActor && HitActor->ActorHasTag(BuildableSurfaceTag);
}

FRotator UKarakuriPlacementComponent::SnapRotationToCardinal(const FRotator& InRotation) const
{
	const float SnappedYaw = FMath::RoundToFloat(InRotation.Yaw / 45.0f) * 45.0f;
	return FRotator(0.0f, SnappedYaw, 0.0f);
}

/*
// null 이면 해당 한변 내에 카라쿠리 없음 null이 아니면 가까운 반경 내 카라쿠리 앵커 스냅 활성
// 월드 내 카라쿠리 액터 순회
ABaseKarakuriActor* UKarakuriPlacementComponent::FindNearestKarakuriAnchor(const FVector& PlayerLocation) const
{
	ABaseKarakuriActor* NearestKarakuri = nullptr;
	float NearestDistSq = FMath::Square(LockedAnchorRadius);
	
	for (TActorIterator<ABaseKarakuriActor> It(GetWorld()); It; ++It)
	{
		ABaseKarakuriActor* Karakuri = *It;
		const float DistSq = FVector::DistSquared(PlayerLocation, Karakuri->GetActorLocation());
		
		if (DistSq < NearestDistSq)
		{
			NearestDistSq = DistSq;
			NearestKarakuri = Karakuri;
		}
	}
	
	
	return NearestKarakuri;
}
*/

void UKarakuriPlacementComponent::SpawnKarakuriActor()
{
	if (!bIsValidSpawnLocation)
	{
		return;
	}
	if (!KarakuriClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("KarakuriClass is not set"));
		return;
	}
	if (KarakuriClass)
	{
		ABaseKarakuriActor* NewKarakuri = GetWorld()->SpawnActor<ABaseKarakuriActor>(KarakuriClass);
		if (NewKarakuri)
		{
			NewKarakuri->Tags.Add(TEXT("KarakuriGround"));
			NewKarakuri->SetActorLocation(GhostActor->GetActorLocation());
			NewKarakuri->SetActorRotation(GhostActor->GetActorRotation());
		}
		if (!bIsSnapLocked)
		{
			OriginLocation = NewKarakuri->GetActorLocation();
			OriginRotation = NewKarakuri->GetActorRotation();
			PlacementCount = 1;
		}
		else
		{
			PlacementCount++;
			TryCompleteDoorRecipe();
		}
	}

	
	
	/*
	 // 마우스 좌표 기준 스폰 액터 코드
	 if (!bIsValidSpawnLocation)
	{
		return;
	}
	if (!KarakuriClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("KarakuriClass is not set"));
		return;
	}

	if (KarakuriClass)
	{
		ABaseKarakuriActor* NewKarakuri = GetWorld()->SpawnActor<ABaseKarakuriActor>(KarakuriClass);
		if (NewKarakuri)
		{
			NewKarakuri->Tags.Add(TEXT("KarakuriGround"));
			NewKarakuri->SetActorLocation(GhostActor->GetActorLocation());
			NewKarakuri->SetActorRotation(GhostActor->GetActorRotation());
		}
	}
	if (bFreePlacementOverride)
	{
		bFreePlacementOverride = false;
	}*/
}
