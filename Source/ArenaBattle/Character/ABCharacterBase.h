// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/ABAnimationAttackInterface.h"
#include "Interface/ABCharacterWidgetInterface.h"
#include "Interface/ABCharacterItemInterface.h"
#include "ABCharacterBase.generated.h"

// 열거형 (입력 컨트롤을 관리하기 위함).
UENUM()
enum class ECharacterControlType : uint8
{
	Shoulder,
	Quarter
};

// 아이템 획등 처리를 위한 델리게이트 선언
DECLARE_DELEGATE_OneParam(FOnTakeItemDelege, class UABItemData* /*InItemData*/);

// 다중 상속: 여러 부모를 상속하는 형태.
// -> 쓰지 마시라.
// -> 따라서 조건부로 잘 사용해야 함.
// -> "클래스 상속"은 하나만 허용.
// -> "나머지 다중 상속은 모두 인터페이스로(순수 가상 함수를 가지는 클래스)만"
// -> 순수 가상 함수를 가진 클래스는 그 자체로는 인스턴스를 생성할 수 없음.
// -> 로우 레벨 기준에서 왜 안될까??? 링커.
UCLASS()
class ARENABATTLE_API AABCharacterBase 
	: public ACharacter, 
	public IABAnimationAttackInterface,
	public IABCharacterWidgetInterface,
	public IABCharacterItemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AABCharacterBase();

	// 공격 감지 함수.
	virtual void AttackHitCheck() override;

	// 대미지 처리 함수.
	virtual float TakeDamage(
		float DamageAmount, 
		struct FDamageEvent const& DamageEvent, 
		class AController* EventInstigator, 
		AActor* DamageCauser) override;


	// Inherited via IABCharacterWidgetInterface
	virtual void SetupCharacterWidget(UABUserWidget* InUserWidget) override;

	// 모든 컴포넌트의 초기화가 끝나면 실행
	virtual void PostInitializeComponents() override;

protected:
	// Dead.
	// 죽음 설정 함수.
	virtual void SetDead();

	// 죽는 애니메이션 재생 함수.
	void PlayDeadAnimation();

protected:
	// 컨트롤 데이터 설정.
	virtual void SetCharacterContolData(
		const class UABCharacterControlData* InCharacterControlData
	);

	// 콤보 공격 처리 함수.
	// 공격을 처음 시작할 때와 콤보 액션을 진행할 때 실행.
	void ProcessComboCommand();

	// 콤보 공격이 시작될 때 실행할 함수.
	void ComboActionBegin();

	// 몽타주 재생 종료 시 호출할 함수 (델리게이트와 연동).
	void ComboActionEnd(UAnimMontage* TargetMontage, bool bInterrupted);

	// 콤보 타이머 설정 함수.
	void SetComboCheckTimer();

	// 콤보 타이밍 처리 함수.
	// 타이머를 설정하고, 설정한 시간을 지나면 실행.
	// 시간 전에 공격 입력이 제대로 들어왔는지 확인.
	void ComboCheck();
	
	// Item
protected:
	// 아이템 습득 시 호출될 함수 (인터페이스를 통해)
	virtual void TakeItem(class UABItemData* ItemData) override;
	
	// 아이템 종류마다 처리될 함수
	virtual void DrinkPotion(UABItemData* InItemData);
	virtual void EquipWeapon(UABItemData* InItemData);
	virtual void ReadScroll(UABItemData* InItemData);
	
	// 아이템 처리를 위한 델리게이트 관리 배열
	TArray<FOnTakeItemDelege> TakeItemActions;
	
protected:
	// 컨트롤 타입 별 데이터 관리를 위한 맵.
	UPROPERTY(EditAnywhere, Category = CharacterControl)
	TMap<ECharacterControlType, class UABCharacterControlData*> CharacterControlManager;

protected:
	// 콤보 공격 몽타주.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attack)
	TObjectPtr<class UAnimMontage> ComboAttackMontage;

	// 콤보 액션 처리 데이터.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attack)
	TObjectPtr<class UABComboActionData> ComboActionData;

	// 현재 콤보 단계 (0은 실행 안된 상태. 1/2/3/4로 단계 구분).
	UPROPERTY(EditAnywhere, Category = Attack)
	uint32 CurrentCombo = 0;

	// 콤보 판정에 사용할 타이머.
	FTimerHandle ComboTimerHandle;

	// 콤보 점프를 판정할 때 사용할 플래그.
	UPROPERTY(VisibleAnywhere, Category = Attack)
	bool bHasNextComboCommand = false;

	// Dead Section.
protected:
	// 죽음 몽타주.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Dead)
	TObjectPtr<class UAnimMontage> DeadMontage;

	// 죽은 후 대기할 시간 값(단위: 초).
	float DeadEventDelayTime = 5.0f;

protected:
	// 스탯 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat")
	TObjectPtr<class UABCharacterStatComponent> Stat;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Widget")
	TObjectPtr<class UABWidgetComponent> HpBar;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Equitment)
	TObjectPtr<class USkeletalMeshComponent> Weapon;	
};
