// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/ABCharacterPlayer.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputSubsystems.h"

#include "InputMappingContext.h"
#include "InputAction.h"
#include "EnhancedInputComponent.h"

#include "ABCharacterControllData.h"

AABCharacterPlayer::AABCharacterPlayer()
{
	// 기본 설정
	// 컨트롤러의 회전 값을 받아서 설정하는 옵션 비활성화
	bUseControllerRotationPitch = false;	// Y축 회전
	bUseControllerRotationYaw = false;		// Z축 회전
	bUseControllerRotationRoll = false;		// X축 회전

	// 무브먼트 설정
	// 캐릭터가 이동하는 방향에 맞게 회전을 해 주는 옵션
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 800.0f;

	//메시 컴포넌트 설정
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.f), FRotator(0.0f, -90.f, 0.0f));

	// 메시 에셋 지정 (검색이 필요)
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CharacterMesh(
		TEXT("/Game/InfinityBladeWarriors/Character/CompleteCharacters/SK_CharM_Golden.SK_CharM_Golden"));

	// 로드 성공했으면 설정
	if (CharacterMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(CharacterMesh.Object);
	}

	//애님 블루프린트 설정
	static ConstructorHelpers::FClassFinder<UAnimInstance>CharacterAnim(
		TEXT("")
	);

	if (CharacterAnim.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(CharacterAnim.Class);;
	}

	// 컴포넌트 생성
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));

	// 계층 설정 (루트 컴포넌트 아래로)
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 600.0f;

	// 컨트롤러의 회전 값을 사용할지 여부
	SpringArm->bUsePawnControlRotation = true;

	// 카메라 컴포넌트
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	// 입력 관련 에셋 로드 및 설정

	static ConstructorHelpers::FObjectFinder<UInputAction>SoulderMoveActionRef(
		TEXT("/Game/ArenaBattle/Input/Actions/IA_ShoulderMove.IA_ShoulderMove")
	);

	if (SoulderMoveActionRef.Succeeded())
	{
		ShoulderMoveAction = SoulderMoveActionRef.Object;
	}


	static ConstructorHelpers::FObjectFinder<UInputAction>ShoulderLookActionRef(
		TEXT("/Game/ArenaBattle/Input/Actions/IA_ShoulderLook.IA_ShoulderLook")
	);

	if (ShoulderLookActionRef.Succeeded())
	{
		ShoulderLookAction = ShoulderLookActionRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction>JumpActionRef(
		TEXT("/Game/ArenaBattle/Input/Actions/IA_Jump.IA_Jump")
	);

	if (JumpActionRef.Succeeded())
	{
		JumpAction = JumpActionRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction>QuarterMoveActionRef(
		TEXT("/Game/ArenaBattle/Input/Actions/IA_QuarterMove.IA_QuarterMove")
	);

	if (QuarterMoveActionRef.Succeeded())
	{
		QuarterMoveAction = QuarterMoveActionRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction>ChangeControlRef(
		TEXT("/Game/ArenaBattle/Input/Actions/IA_ChangeControl.IA_ChangeControl")
	);

	if (ChangeControlRef.Succeeded())
	{
		ChangeControlAction = ChangeControlRef.Object;
	}

	// 기본 컨트롤 설정
	CurrentCharacterControlType = ECharacterContolType::Shoulder;
}

void AABCharacterPlayer::BeginPlay()
{
	Super::BeginPlay();

	// 초기 입력 컨트롤 설정
	SetCharacterControl(CurrentCharacterControlType);
}

void AABCharacterPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// 바인딩 - 향상된 입력 시스템 컴포넌트를 활용해서 설정
	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	if (EnhancedInputComponent)
	{
		EnhancedInputComponent->BindAction(ShoulderMoveAction, ETriggerEvent::Triggered, this, &AABCharacterPlayer::ShoulderMove);
		EnhancedInputComponent->BindAction(ShoulderLookAction, ETriggerEvent::Triggered, this, &AABCharacterPlayer::Look);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &AABCharacterPlayer::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &AABCharacterPlayer::StopJumping);

		// Quarter Move
		EnhancedInputComponent->BindAction(QuarterMoveAction, ETriggerEvent::Triggered, this, &AABCharacterPlayer::QuarterMove);

		// Change Control
		EnhancedInputComponent->BindAction(ChangeControlAction, ETriggerEvent::Started, this, &AABCharacterPlayer::ChangeCharacterControl);

	}
}

void AABCharacterPlayer::SetCharacterControl(ECharacterContolType NewCharacterControlType)
{
	// 변경할 컨트롤 데이터 에셋 로드
	UABCharacterControllData* NewCharacterControl = CharacterControlManager[NewCharacterControlType];

	// 필수로 있어야 하기 때문에 확인
	check(NewCharacterControl);

	// 변경된 속성 설정
	SetCharacterControlData(NewCharacterControl);

	// 입력 매핑 컨텍스트 설정
	APlayerController* PlayerController = Cast<APlayerController>(GetController());

	if (PlayerController)
	{
		// 향상된 입력 시스템의 서브 시스템 가져오기
		UEnhancedInputLocalPlayerSubsystem* InputSystem =
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
				PlayerController->GetLocalPlayer()
			);

		// 향상된 입력 서브 시스템 얻어온 후에 사용할 매핑 컨텍스트 설정
		if (InputSystem)
		{
			// 기존에 설정된 매핑 제거
			InputSystem->ClearAllMappings();

			// 새로운 입력 매핑 컨텍스트 추가
			InputSystem->AddMappingContext(NewCharacterControl->InputMappingContext, 0);
		}
	}

	// 설정이 모두 끝나면 현재 캐릭터 컨트롤 타입 변경
	CurrentCharacterControlType = NewCharacterControlType;
}

void AABCharacterPlayer::SetCharacterControlData(const UABCharacterControllData* InCharacterControlData)
{
	Super::SetCharacterControlData(InCharacterControlData);

	// 나머지 관련 값 설정
	SpringArm->TargetArmLength = InCharacterControlData->TargetArmLength;
	SpringArm->SetRelativeRotation(InCharacterControlData->RelativeRotation);
	SpringArm->bDoCollisionTest = InCharacterControlData->bDoCollisionTest;
	SpringArm->bUsePawnControlRotation = InCharacterControlData->bUsePawnControlRotation;
	SpringArm->bInheritPitch = InCharacterControlData->bInheritPitch;
	SpringArm->bInheritYaw = InCharacterControlData->bInheritYaw;
	SpringArm->bInheritRoll = InCharacterControlData->bInheritRoll;
}

void AABCharacterPlayer::ChangeCharacterControl()
{
	UE_LOG(LogTemp, Log, TEXT("ChangeCharacterControl"));

	// 사용할 캐릭터 컨트롤 변경
	if (CurrentCharacterControlType == ECharacterContolType::Quarter)
	{
		SetCharacterControl(ECharacterContolType::Shoulder);
	}

	else if (CurrentCharacterControlType == ECharacterContolType::Shoulder)
	{
		SetCharacterControl(ECharacterContolType::Quarter);
	}
}

void AABCharacterPlayer::ShoulderMove(const FInputActionValue& Value)
{
	//입력 값 읽어오기 (입력에 지정된 타입으로 변환)
	FVector2D Movement = Value.Get<FVector2D>();

	// 이동할 방향 만들기
	// 카메라가 바라보는 방향 (컨트롤러의 방향)을 기준으로 방햔 만들기
	FRotator Rotation = GetControlRotation();

	//오일러 각 <-> 쿼터니언
	FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);

	// 앞 방향
	FVector ForwardVector = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

	//오른쪽 방향
	FVector RightVector = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	// 무브먼트 컴포넌트에 입력 전달하기 
	AddMovementInput(ForwardVector, Movement.Y);
	AddMovementInput(RightVector, Movement.X);

}

void AABCharacterPlayer::Look(const FInputActionValue& Value)
{
	// 입력 값 가져오기
	FVector2D RotationValue = Value.Get<FVector2D>();

	// 회전 처리
	// 컨트롤러를 회전시키면 스프링 암 컨포넌트가 회전함
	AddControllerYawInput(RotationValue.X);
	AddControllerPitchInput(RotationValue.Y);

}

void AABCharacterPlayer::QuarterMove(const FInputActionValue& Value)
{
	// 입력 값 가져오기
	FVector2D Movement = Value.Get<FVector2D>();

	// 입력 값을 기반으로 이동 방향 만들기
	FVector MoveDirection(Movement.Y, Movement.X, 0.0f);
	MoveDirection.Normalize();

	// 입력 크기 고정 처리 - 대각선 입력이 더 길게 처리되기 때문
	float MovementVectorSize = FMath::Min(1.0f, MoveDirection.Size());

	// 회전 처리 (옵션)
	// 아래 방향 구하는 코드 엄청 어려움 // 방향을 가리키는 회전 행렬을 만들어 줌
	Controller->SetControlRotation(FRotationMatrix::MakeFromX(MoveDirection).Rotator());

	// 이동 적용
	AddMovementInput(MoveDirection, MovementVectorSize);
}
