// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "C_PlayerController.generated.h"

/**
 * 
 */
class UInputMappingContext;
class UInputAction;

UCLASS()
class SPARTA_FEATURE_API AC_PlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	AC_PlayerController();
	virtual void BeginPlay() override;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Input)
	UInputMappingContext* InputMappingContext;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Input)
	UInputAction* MoveAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Input)
	UInputAction* LookAction;
};
