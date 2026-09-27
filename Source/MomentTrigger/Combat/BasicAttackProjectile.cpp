// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicAttackProjectile.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "MomentTriggerCombatLibrary.h"

// Sets default values
ABasicAttackProjectile::ABasicAttackProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));
	RootComponent = CollisionComp;
	CollisionComp->InitSphereRadius(15.0f);
	CollisionComp->OnComponentHit.AddDynamic(this, &ABasicAttackProjectile::OnHit);
	CollisionComp->SetCollisionProfileName(TEXT("BlockAllDynamic"));
	ProjectileMovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComp"));
	ProjectileMovementComp->UpdatedComponent = CollisionComp;
	ProjectileMovementComp->InitialSpeed = ProjectileSpeed;
	ProjectileMovementComp->MaxSpeed = ProjectileSpeed;
	ProjectileMovementComp->ProjectileGravityScale = 0.0f;
	
	// 5초후 자동 소멸
	InitialLifeSpan = 5.0f;

}

void ABasicAttackProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor && OtherActor != this && OtherActor != GetInstigator())
	{
		UAbilitySystemComponent* Player = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(GetInstigator());
		UAbilitySystemComponent* HitOtherActor = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(OtherActor);
		if (Player && HitOtherActor)
		{
			UMomentTriggerCombatLibrary::ApplyDamage(GetInstigator(), OtherActor, DamageAmount, DamageEffectClass);
		}
		UE_LOG(LogTemp, Warning, TEXT("Hit %s"), *OtherActor->GetName());
		Destroy();
	}
}

// Called when the game starts or when spawned
void ABasicAttackProjectile::BeginPlay()
{
	Super::BeginPlay();
	ProjectileMovementComp->Velocity = GetActorForwardVector() * ProjectileSpeed;
	if (APawn* MyInstigator = GetInstigator())
	{
		CollisionComp->IgnoreActorWhenMoving(MyInstigator, true);
	}
}

// Called every frame
void ABasicAttackProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

