// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ABCharacterBase.generated.h"

class UABCharacterControllData;
// 열거형 (현재 컨트롤을 관리하기 위함)
UENUM()
enum class ECharacterContolType :uint8
{
	Shoulder,
	Quarter
};

UCLASS()
class ARENABATTLE_API AABCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AABCharacterBase();

protected:
	// 컨트롤 데이터 설정
	virtual void SetCharacterControlData(const UABCharacterControllData* InCharacterControlData);


protected:
	// 컨트롤 타입별 데이터 관리를 위한 맵
	UPROPERTY(EditAnywhere, Category = "Character Control")
	TMap<ECharacterContolType, UABCharacterControllData*> CharacterControlManager;

};
