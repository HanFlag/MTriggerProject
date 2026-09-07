// Fill out your copyright notice in the Description page of Project Settings.


#include "KarakuriPlacementComponent.h"

#include "KarakuriGhostActor.h"


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
	float PlayerAtAnchorDist = FVector::Dist(OwnerActor->GetActorLocation(), LockedAnchorLocation);
	if (PlayerAtAnchorDist < LockedAnchorRadius)
	{
		bIsSnapLocked = true;
	}
	else
	{
		bIsSnapLocked = false;
	}
	const FRotator SnappedRotation = SnapRotationToCardinal(OwnerActor->GetActorRotation());
	if (!bIsSnapLocked)
	{
		const FVector CandidateLocation = OwnerActor->GetActorLocation() + SnappedRotation.Vector() * GridCellSize;

		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(OwnerActor);
		const FVector TraceStart = CandidateLocation + FVector(0.0f, 0.0f, 200.0f);
		const FVector TraceEnd = CandidateLocation - FVector(0.0f, 0.0f, 200.0f);

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
		const FVector CandidateLocation = LockedAnchorLocation;

		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(OwnerActor);
		const FVector TraceStart = CandidateLocation + FVector(0.0f, 0.0f, 200.0f);
		const FVector TraceEnd = CandidateLocation - FVector(0.0f, 0.0f, 200.0f);

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
		GhostActor->UpdateGhostTransform(GroundHit.ImpactPoint, LockedAnchorRotation);
	}
	
	
}

void UKarakuriPlacementComponent::EndPlacementPreview()
{
	if (GhostActor)
	{
		GhostActor->SetGhostVisible(false);
	}
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
			LockedAnchorLocation = NewKarakuri->GetActorLocation();
			LockedAnchorRotation = NewKarakuri->GetActorRotation();
		}
	}
}

