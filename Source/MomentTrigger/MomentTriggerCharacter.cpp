// Fill out your copyright notice in the Description page of Project Settings.


#include "MomentTriggerCharacter.h"

#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "MomentTriggerAttributeSet.h"
#include "KarakuriPlacementComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AMomentTriggerCharacter::AMomentTriggerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	//SpringArmComp 장착
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->SetupAttachment(RootComponent);
	//SpringArmComp 길이 설정
	SpringArmComp->TargetArmLength = 3600.0f;
	//SpringArmComp 폰 회전에 대한 카메라 회전 비활성화
	SpringArmComp->bUsePawnControlRotation = false;
	//SpringArmComp 절대회전 활성화
	SpringArmComp->SetUsingAbsoluteRotation(true);
	//CameraComp 를 SpringArmComp에 장착
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp);
	
	
	
	//컨트롤러의 회전 적용 설정(컨트롤러가 회전할 때 본체도 함께 회전하지 말아주세요,컨트롤러 회전은 카메라에만 영향을 미치게 해주세요)
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	// 컨트롤러 선호 회전 해제 (우클릭 커서 바라보기에 회전 보간이 적용되게)
	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	//이동방향 자동회전 설정
	// GetCharacterMovement()->bOrientRotationToMovement = true;
	
	// 어빌리티 시스템 컴포넌트 추가
	AbilitySystemComp = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComp->SetIsReplicated(true);
	//소유자 액터와 아바타 액터의 능력 시스템 구성 요소 초기화
	AbilitySystemComp->SetReplicationMode(AscReplicationMode);
	//AbilitySystemComp->InitAbilityActorInfo(this,this);
	AttributeSet = CreateDefaultSubobject<UMomentTriggerAttributeSet>(TEXT("AttributeSet"));

	KarakuriPlacementComp = CreateDefaultSubobject<UKarakuriPlacementComponent>(TEXT("KarakuriPlacementComp"));
}

// Called when the game starts or when spawned
void AMomentTriggerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMomentTriggerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (AbilitySystemComp)
	{
		
		AbilitySystemComp->InitAbilityActorInfo(this, this);
		if (TestAbilityClass)
		{
			AbilitySystemComp->GiveAbility(FGameplayAbilitySpec(TestAbilityClass,1,INDEX_NONE, this));
		}
		if (AttributeSet)
		{
			UE_LOG(LogTemp, Warning, TEXT("Health : %f"), AttributeSet->GetHealth());
		}
	}
}

void AMomentTriggerCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	
	if (AbilitySystemComp)
	{
		AbilitySystemComp->InitAbilityActorInfo(this, this);
	}
}


// Called every frame
void AMomentTriggerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//애니메이션과 속도가 부드럽게 연출하기위해 감속하는 속도 구현
	//그런데 블루프린트에 감속도가 있는데 왜 쓰질 않는가?
	//내가 코드로 스프린트 끝마칠때 속도가 300으로 지정되기때문
	if (bIsDecelerationActive)
	{
		float CurrentMaxSpeed = GetCharacterMovement()->MaxWalkSpeed;
		
		float NewMaxSpeed = FMath::FInterpTo(CurrentMaxSpeed, JogSpeed, DeltaTime, SpeedInterp);
		GetCharacterMovement()->MaxWalkSpeed = NewMaxSpeed;
		if (GetCharacterMovement()->MaxWalkSpeed <= JogSpeed)
		{
			GetCharacterMovement()->MaxWalkSpeed = JogSpeed;
			bIsDecelerationActive = false;
		}
	}
}

// Called to bind functionality to input
void AMomentTriggerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAction(TestAbilityAction,ETriggerEvent::Started,this,&AMomentTriggerCharacter::TestActivateAbility);
	}

}
//컨트롤러의 Sprint상태에 맞게 캐릭터 MovementSpeed에 동기화 -- 주석처리
//감속 할지 안할지 상태 지정
void AMomentTriggerCharacter::SetSprint(bool bEnable)
{
	if (bEnable)
	{
	// GetCharacterMovement()->MaxWalkSpeed = bEnable ? SprintSpeed : JogSpeed;
		bIsDecelerationActive = false;
		GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
	}
	else
	{
		bIsDecelerationActive = true;
	}
}



void AMomentTriggerCharacter::SetMouseLookState(bool bIsMouseLooking)
{
	// 우클릭 중일 때는 이동 방향 회전을 끄고, 떼면 다시 켬
	GetCharacterMovement()->bOrientRotationToMovement = !bIsMouseLooking;
}

void AMomentTriggerCharacter::RotateToTargetLocation(const FVector& TargetLocation)
{
	//플레이어 위치 불러오기
	FVector PlayerLocation = GetActorLocation();
	//타겟팅된 지점을 컨트롤러에서 호출된 커서 임팩트 지점의 X,Y값을 설정,Z값은 플레이어의 위치Z값으로 설정
	FVector TargetWithCharacterZ = FVector(TargetLocation.X, TargetLocation.Y, PlayerLocation.Z);
	//타겟 위치와 플레이어의 위치간의 회전 위치를 찾기
	FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(PlayerLocation, TargetWithCharacterZ);
	// FRotator CurrentRotation = FRotator(0.0f, GetActorRotation().Yaw, 0.0f);
	//플레이어의 회전 불러오기
	FRotator CurrentRotation = GetActorRotation();
	// 회전 보간 계산 및 적용 (Character 본인의 몫)
	FRotator SmoothedRotation = FMath::RInterpTo(
	CurrentRotation,
	TargetRotation,
	GetWorld()->GetDeltaSeconds(),
	RotationInterpSpeed);
	UE_LOG(LogTemp , Warning , TEXT("SmoothedRotation : %f"), SmoothedRotation.Yaw)
	//액터 회전을 SmoothedRotation 회전보간으로 적용 !
	SetActorRotation(SmoothedRotation);

}

void AMomentTriggerCharacter::TestActivateAbility()
{
	if (AbilitySystemComp)
	{
		AbilitySystemComp->TryActivateAbilityByClass(TestAbilityClass);
	}
}

// 어빌리티 함수를 어빌리티comp 로
UAbilitySystemComponent* AMomentTriggerCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComp;
}


