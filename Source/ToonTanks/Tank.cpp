// Fill out your copyright notice in the Description page of Project Settings.


#include "Tank.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

ATank::ATank()
{
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SprinArmComponent"));
	SpringArm->SetupAttachment(RootComponent);
	
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	Camera->SetupAttachment(SpringArm);	
}
void ATank::HandelDestruction()
{
	Super::HandleDestruction();
	SetActorHiddenInGame(true);
	SetActorTickEnabled(false);
	bAlive = false;
}
// Called when the game starts or when spawned
void ATank::BeginPlay()
{
	Super::BeginPlay();

	TankPlayerController = Cast<APlayerController>(GetController());	
}
// Called every frame
void ATank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (TankPlayerController)
	{
		FHitResult HitResult;
		TankPlayerController->GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, HitResult);
		DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 10.f, 12, FColor::Red, false, -1.f);
		RotateTurret(HitResult.ImpactPoint);
	}

}
void ATank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ATank::Move);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &ATank::Turn);

	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &ATank::Fire);
}
void ATank::Move(float Value)
{
	FVector DeltaLocation = FVector::ZeroVector;
	DeltaLocation.X = Value * Speed * UGameplayStatics::GetWorldDeltaSeconds(this);
	AddActorLocalOffset(DeltaLocation, true);
}
void ATank::Turn(float Value)
{
	FRotator DeltaRotation = FRotator::ZeroRotator;
	DeltaRotation.Yaw = Value * TurnRate * UGameplayStatics::GetWorldDeltaSeconds(this);
	AddActorLocalRotation(DeltaRotation, true);
}
