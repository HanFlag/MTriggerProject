// Fill out your copyright notice in the Description page of Project Settings.


#include "KarakuriGhostActor.h"

#include "Components/StaticMeshComponent.h"

AKarakuriGhostActor::AKarakuriGhostActor()
{
	PrimaryActorTick.bCanEverTick = false;

	PreviewMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PreviewMesh"));
	SetRootComponent(PreviewMesh);
	PreviewMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	SetActorHiddenInGame(true);
}

void AKarakuriGhostActor::SetGhostVisible(bool bVisible)
{
	SetActorHiddenInGame(!bVisible);
}

void AKarakuriGhostActor::UpdateGhostTransform(const FVector& NewLocation, const FRotator& NewRotation)
{
	SetActorLocationAndRotation(NewLocation, NewRotation);
}
