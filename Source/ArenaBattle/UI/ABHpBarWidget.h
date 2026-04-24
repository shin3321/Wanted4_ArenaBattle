// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ABUserWidget.h"
#include "ABHpBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class ARENABATTLE_API UABHpBarWidget : public UABUserWidget
{
	GENERATED_BODY()

public:
	// UserWidget은 기본 생성자가 아닌 FObjectInitializer를 파라미터로 받는 생성자를 사용함
	UABHpBarWidget(const FObjectInitializer& ObjectInitializer);

	//Setter
	FORCEINLINE void SetMaxHp(float NewMaxHp) { MaxHp = NewMaxHp; }

	// 체력 변경 이벤트(델리게이트)에 등록할 함수
	void UpdateHpBar(float NewCurrentHp);

protected:
	// UMG가 
	virtual void NativeConstruct() override;

protected:
	//hp 게이지를 보여 주기 위해 사용할 프로그레스 바
	UPROPERTY()
	TObjectPtr<class UProgressBar> HpProgressBar;

	// 최대 체력 값 설정 (게이지 계산을 위해서) 
	UPROPERTY() 
	float MaxHp;
};
