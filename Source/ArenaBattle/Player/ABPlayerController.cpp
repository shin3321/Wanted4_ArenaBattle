// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/ABPlayerController.h"

void AABPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// 입력 모드 설정
	// 게잉ㅁ 시작되면 뷰포트로 바ㅓ로 입력되도ㄽㄱ
	FInputModeGameOnly GameOnlyInputMode;
	SetInputMode(GameOnlyInputMode);
	//stsic ConstructorHelpers::FClassFinder<APlayerContolier PlayerConstruct(Text("z"))
}
