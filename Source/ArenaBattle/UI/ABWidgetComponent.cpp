// Fill out your copyright notice in the Description page of Project Settings.


#include "ABWidgetComponent.h"
#include "ABUserWidget.h"

void UABWidgetComponent::InitWidget()
{
	Super::InitWidget();

	// Super::InitWidget() 상위 로직을 따라가 보면 함수 실행 과정에서 Create Widget을 통해 위젯이 생성됨
	// 그 이후에 여기가 실행 됨 따라서 위젯 촉기화를 보장받을 수 있음

	UABUserWidget* ABUserWidget = Cast<UABUserWidget>(GetWidget());
	if (ABUserWidget)
	{
		ABUserWidget->SetOwningActor(GetOwner());
	}
}
