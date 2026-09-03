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
	
	
	
	//컨트롤러의 회전 적용 설정
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	// 컨트롤러 선호 회전 해제 (우클릭 커서 바라보기에 회전 보간이 적용되게)
	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	//이동방향 자동회전 설정
	// GetCharacterMovement()->bOrientRotationToMovement = true;
}

// Called when the game starts or when spawned
void AMomentTriggerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// void AMomentTriggerCharacter::Move(const FInputActionValue& Value)
// {
// 	FVector2D MovementVector = Value.Get<FVector2D>();
//
// 	// route the input
// 	DoMove(MovementVector.X, MovementVector.Y);
// }


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

	FVector PlayerLocation = GetActorLocation();
	FVector TargetWithCharacterZ = FVector(TargetLocation.X, TargetLocation.Y, PlayerLocation.Z);
	FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(PlayerLocation, TargetWithCharacterZ);
	// FRotator CurrentRotation = FRotator(0.0f, GetActorRotation().Yaw, 0.0f);
	FRotator CurrentRotation = GetActorRotation();
	// 회전 보간 계산 및 적용 (Character 본인의 몫)
	FRotator SmoothedRotation = FMath::RInterpTo(
	CurrentRotation,
	TargetRotation,
	GetWorld()->GetDeltaSeconds(),
	RotationInterpSpeed);
	UE_LOG(LogTemp , Warning , TEXT("SmoothedRotation : %f"), SmoothedRotation.Yaw)
	
	SetActorRotation(SmoothedRotation);

}

// void AMomentTriggerCharacter::RotateToTargetLocation(const FVector& TargetLocation)
// {
// 	FVector PlayerLocation = GetActorLocation();
// 	
// 	
// 	FVector TargetWithCharacterZ = FVector(TargetLocation.X, TargetLocation.Y, PlayerLocation.Z);
// 	
// 	//마우스가 너무 가까우면 확확 회전하는거 생략
// 	if (FVector::DistSquared2D(PlayerLocation, TargetWithCharacterZ) < 100.0f)
// 	{
// 		return;
// 	}
// 	FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(PlayerLocation, TargetWithCharacterZ);
// 	float CurrentYaw = GetActorRotation().Yaw;
// 	
// 	float DeltaYaw = FMath::FindDeltaAngleDegrees(CurrentYaw, TargetRotation.Yaw);
// 	float NewYaw = CurrentYaw + (DeltaYaw * GetWorld()->GetDeltaSeconds() * RotationInterpSpeed);
// 	// FRotator CurrentRotation = FRotator(0.0f, GetActorRotation().Yaw, 0.0f);
// 	
// 	// FRotator CurrentRotation = GetActorRotation();
// 	// // 회전 보간 계산 및 적용 (Character 본인의 몫)
// 	// FRotator SmoothedRotation = FMath::RInterpTo(
// 	// 	CurrentRotation,
// 	// 	TargetRotation,
// 	// 	GetWorld()->GetDeltaSeconds(),
// 	// 	RotationInterpSpeed);	
// 	UE_LOG(LogTemp , Warning , TEXT("SmoothedRotation : %f"), NewYaw)
// 	SetActorRotation(FRotator(0.0f, NewYaw, 0.0f));
// }

//캐릭터 의 컨트롤중인 폰 캐스트 -> lock 트리거 동안 회전보간 끄기
//우클릭시 마우스 회전벡터 로 캐릭터 방향 바라보기 기능
//캐릭터 위치에서 커서 포인트 까지 회전값 계산
// Yaw만 적용 -> 플레이어의 메쉬와 방향 확인하기
//부드러운 플레이어의 회전 보간

