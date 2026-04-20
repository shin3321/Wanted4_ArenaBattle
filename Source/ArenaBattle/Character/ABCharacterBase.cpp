// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ABCharacterBase.h"
#include "ABCharacterControllData.h"
#include "GameFramework/CharacterMovementComponent.h"


// Sets default values
AABCharacterBase::AABCharacterBase()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UABCharacterControllData> ShoulderDataRef(TEXT("/Game/ArenaBattle/CharacterControl/ABC_Soulder.ABC_Soulder"));

	if (ShoulderDataRef.Succeeded())
	{
		CharacterControlManager.Add(ECharacterContolType::Shoulder, ShoulderDataRef.Object);
	}
	static ConstructorHelpers::FObjectFinder<UABCharacterControllData> QuarterDataRef(TEXT("/Game/ArenaBattle/CharacterControl/ABC_Quater.ABC_Quater"));

	if (QuarterDataRef.Succeeded())
	{
		CharacterControlManager.Add(ECharacterContolType::Quarter, QuarterDataRef.Object);
	}
}


void AABCharacterBase::SetCharacterControlData(const UABCharacterControllData* InCharacterControlData)
{
	// Pawn
	bUseControllerRotationYaw = InCharacterControlData->bUseControllerRotationYaw;

	// Character Movement
	GetCharacterMovement()->bUseControllerDesiredRotation = InCharacterControlData->bUseControllerDesiredRotation;

	GetCharacterMovement()->bOrientRotationToMovement = InCharacterControlData->bOrientRotationToMovement;
	GetCharacterMovement()->RotationRate = InCharacterControlData->RotationRate;;
}
