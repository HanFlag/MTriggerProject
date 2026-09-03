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





