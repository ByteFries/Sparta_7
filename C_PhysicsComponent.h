// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "C_PhysicsComponent.generated.h"


UCLASS( ClassGroup=(Custom), Blueprintable, meta=(BlueprintSpawnableComponent) )
class MYPHYSICSRUNTIME_API UC_PhysicsComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UC_PhysicsComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Physics")
	FVector Force;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Physics")
	FVector Velocity;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Physics")
	FVector Acceleration;
private:
	UFUNCTION(BlueprintCallable, Category="Physics")
	void Calculate();
	UFUNCTION(BlueprintCallable, Category="Physics")
	UMeshComponent* GetOwnerMesh();

	

	FVector LocalVelocity;


	FQuat Rotation;
	FVector MomentSum;
	FVector AngularVelocity;
	FVector LocalAngularVelocity;
	FVector LocalAngularAcceleration;
	
	FMatrix InertiaTensor;
	FMatrix LocalInertiaTensor;
	FMatrix InverseInertiaTensor;
	FMatrix WorldInverseInertiaTensor;
};
