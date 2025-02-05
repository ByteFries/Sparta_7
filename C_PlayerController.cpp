// Fill out your copyright notice in the Description page of Project Settings.


#include "C_PlayerController.h"
#include "EnhancedInputSubsystems.h"

AC_PlayerController::AC_PlayerController()
{
}

void AC_PlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (ULocalPlayer* player = GetLocalPlayer())
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = player->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			if (InputMappingContext)
			{
				Subsystem->AddMappingContext(InputMappingContext, 0);
			}
		}
	}
}
