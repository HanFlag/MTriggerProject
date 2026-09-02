// Fill out your copyright notice in the Description page of Project Settings.


#include "MomentTriggerPlayerController.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputComponent.h"
#include "MomentTriggerCharacter.h"
#include "GameFramework/SpringArmComponent.h"


AMomentTriggerPlayerController::AMomentTriggerPlayerController()
{
	bShowMouseCursor = true;
}

void AMomentTriggerPlayerController::BeginPlay()
{
	Super::BeginPlay();
	// only spawn touch controls on local player controllers
	//상투적인 코드
	ULocalPlayer* LocalPlayer = GetLocalPlayer();
	if (LocalPlayer)
	{
		UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<
			UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);
		if (Subsystem)
		{
			if (DefaultInputMappingContext)
				Subsystem->AddMappingContext(DefaultInputMappingContext, 0);
		}
	}
}

void AMomentTriggerPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	// only add IMCs for local player controllers
	// 상투적으로 MoveAction Triggered형식으로
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMomentTriggerPlayerController::Move);
		EnhancedInputComponent->BindAction(MouseLock, ETriggerEvent::Triggered, this, &AMomentTriggerPlayerController::MouseLockTrigger);EnhancedInputComponent->BindAction(MouseLock, ETriggerEvent::Completed, this, &AMomentTriggerPlayerController::MouseLockComplated);
		
	}
		
}

void AMomentTriggerPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	
}

void AMomentTriggerPlayerController::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();
	
	if (APawn* ControlledPawn = GetPawn())
	{
		// ControlledPawn->AddMovementInput(FVector::ForwardVector, MovementVector.Y);
		// ControlledPawn->AddMovementInput(FVector::RightVector, MovementVector.X);

		// 스프링암 정보를 가져오기 위해서 GetPawn 조종중인 것을 불러오기
		if (AMomentTriggerCharacter* TargetCharacter = Cast<AMomentTriggerCharacter>(GetPawn()))
		{
			// 유효 하다면
			if (TargetCharacter)
			{
				// 스프링 암 컴포넌트의 회전값 Yaw를 추출
				FRotator SpringArmRotator = TargetCharacter->SpringArmComp->GetComponentRotation();
				FRotator YawRotation(0.0f, SpringArmRotator.Yaw, 0.0f);


				// 스프링암 기준으로 벡터 계산
				const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
				const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
				//AddMovementInput
				ControlledPawn->AddMovementInput(ForwardDirection, MovementVector.X);
				ControlledPawn->AddMovementInput(RightDirection, MovementVector.Y);
			}
		}
	}
}

void AMomentTriggerPlayerController::MouseLockTrigger()
{
	//캐릭터 의 컨트롤중인 폰 캐스트 -> lock 트리거 동안 회전보간 끄기
	//우클릭시 마우스 회전벡터 로 캐릭터 방향 바라보기 기능
	if (AMomentTriggerCharacter* TargetCharacter = Cast<AMomentTriggerCharacter>(GetPawn()))
	{
		TargetCharacter->GetCharacterMovement()->bOrientRotationToMovement = false;
	}
	APawn* ControlledPawn = GetPawn();
	if (ControlledPawn)
	{
		
		FHitResult HitResult;
		
		if (GetHitResultUnderCursor(ECC_Visibility, false, HitResult))
		{
			//캐릭터 위치에서 커서 포인트 까지 회전값 계산
			FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(ControlledPawn->GetActorLocation(), HitResult.ImpactPoint);
			// Yaw만 적용 -> 플레이어의 메쉬와 방향 확인하기
			UE_LOG(LogTemp, Display, TEXT("Yaw : %f"), TargetRotation.Yaw);
			FRotator NewRotation = FRotator(0.0f, TargetRotation.Yaw, 0.0f);
			ControlledPawn->SetActorRotation(NewRotation);
		}
	}
}
// 우클릭 뗄시 다시 캐릭터 이동에 회전 보간
void AMomentTriggerPlayerController::MouseLockComplated()
{
	if (AMomentTriggerCharacter* TargetCharacter = Cast<AMomentTriggerCharacter>(GetPawn()))
	{
		TargetCharacter->GetCharacterMovement()->bOrientRotationToMovement = true;
	}
}


