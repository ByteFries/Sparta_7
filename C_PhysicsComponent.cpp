// Fill out your copyright notice in the Description page of Project Settings.


#include "C_PhysicsComponent.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Tests/AutomationCommon.h"

// Sets default values for this component's properties
UC_PhysicsComponent::UC_PhysicsComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	
	// ...
}


// Called when the game starts
void UC_PhysicsComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	auto Mesh = GetOwnerMesh();
	auto Inertia = Mesh->GetInertiaTensor();

	InertiaTensor = FMatrix(
	FVector(Inertia.X, 0, 0), // 첫 번째 행
	FVector(0, Inertia.Y, 0), // 두 번째 행
	FVector(0, 0, Inertia.Z), // 세 번째 행
FVector(0.f, 0.f, 0.f)       
	);
}


// Called every frame
void UC_PhysicsComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	AActor* Owner = GetOwner();

	if (Owner)
	{
		Calculate();
		
		//Owner->SetActorRotation(Rotation);
		FHitResult HitResult;
		FVector StartLocation = Owner->GetActorLocation();
		
		FVector Offset = FVector(0, 0, -50);
		FVector EndLocation = StartLocation + Offset;
		if (!GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility))
		{
			Owner->SetActorLocation(Owner->GetActorLocation() + (Velocity * DeltaTime));
		}
		else
		{
			Velocity = FVector(0,0,0);
			Force = FVector(0,0,0);
			Acceleration = FVector(0,0,0);
			UE_LOG(LogTemp, Warning, TEXT("LineTraceSingleByChannel failed"));
		}
	}
	// ...
}

void UC_PhysicsComponent::Calculate()
{
	// 공기 저항? FD = 0.5 * p * v^2 * Cd * A 항력 계수와 단면적 구하기가 없어서 그냥 댐핑 사용해?
	float DeltaTime = GetWorld()->GetDeltaSeconds();
	
	auto Mesh = GetOwnerMesh();
	float Mass = Mesh->GetMass() == 0 ? 1 : Mesh->GetMass();
	Force.Z -= 9.8 * Mass;
	Acceleration = Force / Mass;
	Velocity = Acceleration * DeltaTime;
	
	//각운동량
	/*FVector GyroEffect = FVector::CrossProduct(LocalAngularVelocity, InertiaTensor.TransformVector(LocalAngularVelocity));

	LocalAngularAcceleration = InverseInertiaTensor.TransformVector(MomentSum - GyroEffect);

	LocalAngularVelocity += LocalAngularAcceleration * DeltaTime;

	float Angle = LocalAngularVelocity.Length() * (0.5f * DeltaTime);
	FVector Axis = LocalAngularVelocity.GetSafeNormal();

	if (Angle > FLT_EPSILON)
	{
		FQuat DeltaRotation = FQuat(Axis, Angle);
		Rotation = (Rotation * DeltaRotation).GetNormalized();
	}

	LocalVelocity = Rotation.Inverse().RotateVector(Velocity);
	AngularVelocity = Rotation.RotateVector(LocalAngularVelocity);

	// 회전 크기 정규화
	if (Rotation.Size() > FLT_EPSILON)
	{
		Rotation.Normalize();
	}

	// 회전 행렬 구하기
	FMatrix R = Rotation.ToMatrix(); // 쿼터니언에서 회전 행렬로 변환
	FMatrix RT = R.GetTransposed(); // 회전 행렬의 전치 구하기

	// 역관성 텐서 계산
	WorldInverseInertiaTensor = R * InverseInertiaTensor * RT;
	*/
}

UMeshComponent* UC_PhysicsComponent::GetOwnerMesh()
{
	if (AActor* OwnerActor = GetOwner())
	{
		return OwnerActor->FindComponentByClass<UMeshComponent>();
	}
	return nullptr;
}
