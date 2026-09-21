// Fill out your copyright notice in the Description page of Project Settings.


#include "ABasicAttackProjectile.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffect.h"
#include "Combat/MomentTriggerGameplayTags.h"
#include "AbilitySystemGlobals.h"

// Sets default values
AABasicAttackProjectile::AABasicAttackProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));
	RootComponent = CollisionComp;
	CollisionComp->InitSphereRadius(15.0f);
	CollisionComp->OnComponentHit.AddDynamic(this, &AABasicAttackProjectile::OnHit);
	CollisionComp->SetCollisionProfileName(TEXT("BlockAllDynamic"));
	ProjectileMovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComp"));
	ProjectileMovementComp->UpdatedComponent = CollisionComp;
	ProjectileMovementComp->InitialSpeed = ProjectileSpeed;
	ProjectileMovementComp->MaxSpeed = ProjectileSpeed;
	ProjectileMovementComp->ProjectileGravityScale = 0.0f;
	
	// 5초후 자동 소멸
	InitialLifeSpan = 5.0f;

}

void AABasicAttackProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor && OtherActor != this && OtherActor != GetInstigator())
	{
		UAbilitySystemComponent* Player = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(GetInstigator());
		UAbilitySystemComponent* HitOtherActor = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(OtherActor);
		if (Player && HitOtherActor)
		{
			FGameplayEffectContextHandle ContextHandle = Player->MakeEffectContext();
			FGameplayEffectSpecHandle SpecHandle = Player->MakeOutgoingSpec(DamageEffectClass, 1.0f, ContextHandle);
			if (!SpecHandle.IsValid())
			{
				UE_LOG(LogTemp, Warning, TEXT("DamageEffectClass is not valid"));
			}
			else
			{
				SpecHandle.Data->SetSetByCallerMagnitude(TAG_Data_Damage, -DamageAmount);
				Player->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), HitOtherActor);
			}
			
		}
		UE_LOG(LogTemp, Warning, TEXT("Hit %s"), *OtherActor->GetName());
		Destroy();
	}
}

// Called when the game starts or when spawned
void AABasicAttackProjectile::BeginPlay()
{
	Super::BeginPlay();
	ProjectileMovementComp->Velocity = GetActorForwardVector() * ProjectileSpeed;
	if (APawn* MyInstigator = GetInstigator())
	{
		CollisionComp->IgnoreActorWhenMoving(MyInstigator, true);
	}
}

// Called every frame
void AABasicAttackProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

