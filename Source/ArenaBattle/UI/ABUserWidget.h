// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ABUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class ARENABATTLE_API UABUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetOwningActor(AActor* NewActor){ OwningActor = NewActor;}
	
protected:
	// 이 위젯을 소유하는 액터 정보
	UPROPERTY(visibleAnywhere, BlueprintReadOnly, category = "Actor")	
	TObjectPtr<AActor> OwningActor;

};
