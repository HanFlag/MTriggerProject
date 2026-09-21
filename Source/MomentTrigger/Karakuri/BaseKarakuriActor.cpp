// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseKarakuriActor.h"

#include "KarakuriPlacementComponent.h"
#include "StructUtils/PropertyBag.h"

// Sets default values
ABaseKarakuriActor::ABaseKarakuriActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComp"));
	RootComponent = SceneComp;
	CollisionComp = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionComp"));
	CollisionComp->SetupAttachment(RootComponent);
	CollisionComp->SetBoxExtent(FVector(25.0f,0.0f,50.0f));
	CollisionComp->SetCollisionProfileName(TEXT("BlockAll"));
	
	CurrentHP = MaxHP;

}

// Called when the game starts or when spawned
void ABaseKarakuriActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABaseKarakuriActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseKarakuriActor::SetClusterInfo(const FVector& Origin, const FRotator& Rotation, int32 Index)
{
	ClusterOrigin = Origin;
	ClusterRotation = Rotation;
	ClusterIndex = Index;
}

float ABaseKarakuriActor::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
	class AController* EventInstigator, AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	
	if (bIsDestroyed)
	{
		return ActualDamage;
	}
	CurrentHP -= ActualDamage;
	if (CurrentHP <= 0.0f)
	{
		bIsDestroyed = true;
		OnKarakuriDestroyed();
		Destroy();
	}
	return ActualDamage;
	
}