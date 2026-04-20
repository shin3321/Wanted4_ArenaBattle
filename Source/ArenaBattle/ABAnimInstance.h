// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "ABAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class ARENABATTLE_API UABAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	// 생성자
	UABAnimInstance();

protected:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character");
	TObjectPtr<class ACharacter> Owner;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character");
	TObjectPtr<class UCharacterMovementComponent> Movement;

	// Idel- Move 전환을 위해 사용할 속도
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character");
	FVector Velocity;

	// 이동 빠르기 (블렌드 스페이스에 적용
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character");
	float GroundSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character");
	uint8 bIsIdle : 1;

	// 이동 여부를 판단할 때 사용할 문턱 값
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character");
	float MovingThreshold;

	// 점프 확인용 변수 (떨어질 때)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character");
	uint8 bIsFalling : 1;

	// 점프할 때
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character");
	uint8 bIsJumping : 1;

	// 점프 여부를 판단할 때 사용할 문턱 값
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character");
	float JumpingThreshold;
};
