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

}

// Called to bind functionality to input
void AMomentTriggerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

