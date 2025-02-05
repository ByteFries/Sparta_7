// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "C_Drone.generated.h"

class USpringArmComponent;
class UCameraComponent;
class USkeletalMeshComponent;
class UBoxComponent;
UCLASS()
class SPARTA_FEATURE_API AC_Drone : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AC_Drone();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void UpdateLocation();
	void UpdateRotation();

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	
	
	UFUNCTION()
	void Move(const FInputActionValue& value);
	UFUNCTION()
	void Look(const FInputActionValue& value);

public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Component")
	USkeletalMeshComponent* SkeletalMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Component")
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Component")
	UCameraComponent* CameraComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Component")
	UBoxComponent* BoxComponent;
	

private:
	FRotator Orientation;
	float NormalSpeed;
	float SprintSpeedMultiplier;
	float SprintSpeed;
};