// Fill out your copyright notice in the Description page of Project Settings.


#include "Tower.h"
#include "Tank.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

void ATower::BeginPlay()
{
	Super::BeginPlay();
	Tank = Cast<ATank>(UGameplayStatics::GetPlayerPawn(this, 0));
	GetWorldTimerManager().SetTimer(FireRateTimerHandle, this, &ATower::CheckFireCondidition, FireRate, true);
}
void ATower::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (InFireRanger())
	{
		RotateTurret(Tank->GetActorLocation());
	}
}
void ATower::HandelDestruction()
{
	Super::HandleDestruction();
	Destroy();
}
void ATower::CheckFireCondidition()
{
	if(Tank == nullptr)
	{
		return;
	}
	if (InFireRanger() && Tank->bAlive)
	{
		Fire();
	}
}
bool ATower::InFireRanger()
{
	if (Tank)
	{
		float Distance = FVector::Dist(GetActorLocation(), Tank->GetActorLocation());
		if (Distance <= FireRange)
		{
			return true;
		}
	}
	return false;
}