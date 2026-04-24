// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ABItemData.generated.h"

// 할 얘기: 데이터를 파일(에셋)로 저장해서 관리하면 뭐가 좋을까?
// 생성이 됐는지 안 됐는지 확인할 필요가 없다

// 열거형
UENUM(BlueprintType)
enum class EItemType :uint8
{
	Weapon = 0,
	Potion,
	Scroll
};

UCLASS()
class ARENABATTLE_API UABItemData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Type)
	EItemType Type;
};
