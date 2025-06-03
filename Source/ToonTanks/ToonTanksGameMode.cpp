// Fill out your copyright notice in the Description page of Project Settings.


#include "ToonTanksGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Tank.h"
#include "Tower.h"
#include "ToonTanksPlayerController.h"

void AToonTanksGameMode::ActorDied(AActor* DeadActor)
{
	if (DeadActor == Tank)
	{
		Tank->HandelDestruction();

		if (ToonTanksPlayerController)
		{
			/*Tank->DisableInput(Tank->GetTankPlayerController());
			Tank->GetTankPlayerController()->bShowMouseCursor = false;*/
			ToonTanksPlayerController->SetPlayerEnabledState(false);
		}
		GameOver(false);
	}
	else if (ATower* DestroyedTower = Cast<ATower>(DeadActor))
	{
		DestroyedTower->HandelDestruction();
		--TargetTower;
		if (TargetTower == 0)
		{
			GameOver(true);
		}
	}
}
void AToonTanksGameMode::BeginPlay()
{
	Super::BeginPlay();
	//...
	HandleGameStart();
}
void AToonTanksGameMode::HandleGameStart()
{
	TargetTower = GetTargetTowerCount();
	Tank = Cast<ATank>(UGameplayStatics::GetPlayerPawn(this, 0));
	ToonTanksPlayerController = Cast<AToonTanksPlayerController>(UGameplayStatics::GetPlayerController(this, 0));

	StartGame();

	if (ToonTanksPlayerController)
	{
		ToonTanksPlayerController->SetPlayerEnabledState(false);

		FTimerHandle PlayerEnableTimeHandle;

		FTimerDelegate PlayerEnableTimeDelegate = FTimerDelegate::CreateUObject(ToonTanksPlayerController, &AToonTanksPlayerController::SetPlayerEnabledState, true);

		GetWorldTimerManager().SetTimer(PlayerEnableTimeHandle, PlayerEnableTimeDelegate, StartDeplay, false);
	}
}
int32 AToonTanksGameMode::GetTargetTowerCount()
{
	TArray<AActor*> Tower;
	UGameplayStatics::GetAllActorsOfClass(this, ATower::StaticClass(), Tower);
	return Tower.Num();
}