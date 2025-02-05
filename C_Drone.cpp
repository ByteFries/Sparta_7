// Fill out your copyright notice in the Description page of Project Settings.


#include "C_Drone.h"
#include "Components/BoxComponent.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "C_PlayerController.h"

// Sets default values
AC_Drone::AC_Drone()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	BoxComponent = CreateDefaultSubobject<UBoxComponent>("Box Component");
	BoxComponent->InitBoxExtent({20, 20, 20});
	BoxComponent->SetSimulatePhysics(false);
	RootComponent = BoxComponent;
	
	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>("StaticMesh");
	SkeletalMesh->SetSimulatePhysics(false);
	SkeletalMesh->SetupAttachment(RootComponent);

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("Spring Arm");
	SpringArmComponent->SetupAttachment(RootComponent);

	SpringArmComponent->TargetArmLength = 300.0f;
	SpringArmComponent->bUsePawnControlRotation = false;
	
	CameraComponent = CreateDefaultSubobject<UCameraComponent>("Camera");
	CameraComponent->SetupAttachment(SpringArmComponent);
	CameraComponent->bUsePawnControlRotation = false;
}

// Called when the game starts or when spawned
void AC_Drone::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AC_Drone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	UpdateLocation();
	UpdateRotation();
}

void AC_Drone::UpdateLocation()
{
	FRotator CurrentRotation = SkeletalMesh->GetComponentRotation();
	FRotationMatrix RotationMatrix = FRotationMatrix(CurrentRotation);

	FVector ForwardVector = -RotationMatrix.GetUnitAxis(EAxis::X);
	ForwardVector.Z = 0.f;
	FVector RightVector = RotationMatrix.GetUnitAxis(EAxis::Y);
	RightVector.Z = 0.f;
	
	float MoveSpeed = 10.f;
	FVector MoveDirection = (ForwardVector * (CurrentRotation.Pitch) + RightVector * (CurrentRotation.Roll)) * MoveSpeed * GetWorld()->DeltaTimeSeconds;
	
	SetActorLocation(GetActorLocation() + MoveDirection);
}

void AC_Drone::UpdateRotation()
{
	FRotator CurrentRotation = SkeletalMesh->GetRelativeRotation();
	FRotator TargetRotation = FRotator(0.f, -90.f, 0.f);
	FRotator Diff = CurrentRotation - TargetRotation;

	float PitchInterpSpeed = FMath::Abs(Diff.Pitch) / 350.f;
	float YawInterpSpeed = 0.0f;//FMath::Abs(Diff.Yaw) / 500.f;
	float RollInterpSpeed = FMath::Abs(Diff.Roll) / 350.f;

	PitchInterpSpeed = FMath::Clamp(PitchInterpSpeed, 0.f, 1.f);
	YawInterpSpeed = FMath::Clamp(YawInterpSpeed, 0.f, 1.f);
	RollInterpSpeed = FMath::Clamp(RollInterpSpeed, 0.f, 1.f);

	FRotator NewRotation;
	NewRotation.Pitch = FMath::Lerp(CurrentRotation.Pitch, TargetRotation.Pitch, PitchInterpSpeed);
	NewRotation.Roll = FMath::Lerp(CurrentRotation.Roll, TargetRotation.Roll, RollInterpSpeed);
	NewRotation.Yaw = -90.f;
	SkeletalMesh->SetRelativeRotation(NewRotation);
}

// Called to bind functionality to input
void AC_Drone::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (AC_PlayerController* PlayerController = Cast<AC_PlayerController>(GetController()))
		{
			EnhancedInputComponent->BindAction(
			PlayerController->MoveAction,
			ETriggerEvent::Triggered,
			this,
			&AC_Drone::Move
			);

			EnhancedInputComponent->BindAction(
			PlayerController->LookAction,
			ETriggerEvent::Triggered,
			this,
			&AC_Drone::Look
			);
		}
	}
}


void AC_Drone::Move(const FInputActionValue& value) 
{
	if (!Controller) return;
	const FVector MoveInput = value.Get<FVector>();	
	const FRotator Rot = FRotator(MoveInput.Y,  0, MoveInput.X); 
	SkeletalMesh->AddLocalRotation(Rot);	
	
	SetActorLocation(GetActorLocation() + FVector(0,0,MoveInput.Z));
}

void AC_Drone::Look(const FInputActionValue& value)
{
	//FVector LookInput = value.Get<FVector>();
	//FRotator NewRotator = FRotator(LookInput.Y, LookInput.X, LookInput.Z);
	//AddActorLocalRotation(NewRotator);
	//UE_LOG(LogTemp, Warning, TEXT("BoxComponent Rotation: %s"), *BoxComponent->GetComponentRotation().ToString());

	FVector LookInput = value.Get<FVector>();
	FRotator LookRotator = FRotator(0, LookInput.X, 0);
	//SpringArmComponent->AddWorldRotation(LookRotator);
	AddActorLocalRotation(LookRotator);
}