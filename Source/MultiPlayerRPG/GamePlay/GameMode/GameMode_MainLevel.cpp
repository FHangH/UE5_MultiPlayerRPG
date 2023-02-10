#include "GameMode_MainLevel.h"
#include "MultiPlayerRPG/GamePlay/GameState/GameState_FH.h"
#include "MultiPlayerRPG/GamePlay/PlayerController/PlayerController_FH.h"
#include "MultiPlayerRPG/GamePlay/PlayerState/PlayerState_FH.h"
#include "MultiPlayerRPG/GamePlay/GameInstance/GameInstanceRPG.h"

AGameMode_MainLevel::AGameMode_MainLevel()
{
	PrimaryActorTick.bCanEverTick = false;
	bUseSeamlessTravel = true;
	PlayerControllerClass = APlayerController_FH::StaticClass();
	GameStateClass = AGameState_FH::StaticClass();
	PlayerStateClass = APlayerState_FH::StaticClass();
}

void AGameMode_MainLevel::BeginPlay()
{
	Super::BeginPlay();
	
	Fh_GameInstance = Fh_GameInstance == nullptr ? Cast<UGameInstanceRPG>(GetGameInstance()) : Fh_GameInstance;
	if (Fh_GameInstance)
	{
		Connector = Connector == nullptr ? Fh_GameInstance->FH_GetMySQLConnector() : Connector;
	}
}

void AGameMode_MainLevel::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	
	const auto PC = Cast<APlayerController_FH>(NewPlayer);
	if (PC)
	{
		PC->PCIndex = Index;
		Fh_MapPlayerController.Add(Index, PC);
		++Index;
		PC->FH_Init();
		PC->FH_DelaySpawnPlayer();
		PC->FH_StartUpdateLocationTimer();
	}
}

void AGameMode_MainLevel::Logout(AController* Exiting)
{
	const auto PC = Cast<APlayerController_FH>(Exiting);
	if (PC)
	{
		Fh_MapPlayerController.Remove(PC->PCIndex);
		FH_UpdateUserLoginStatusByPC(PC, false);
		PC->FH_ClearUpdateLocationTimer();
	}
	
	Super::Logout(Exiting);
}

void AGameMode_MainLevel::Destroyed()
{
	Super::Destroyed();
}

/* My Code */
void AGameMode_MainLevel::FH_UpdateUserLoginStatusByPC(APlayerController_FH* PC, const bool IsLogin)
{
	Fh_GameInstance = Fh_GameInstance == nullptr ? Cast<UGameInstanceRPG>(GetGameInstance()) : Fh_GameInstance;
	if (Fh_GameInstance)
	{
		Connector = Connector == nullptr ? Fh_GameInstance->FH_GetMySQLConnector() : Connector;
		if (Connector)
		{
			PC->FH_UpdateUserLoginStatusToMySQL(Connector, IsLogin);
		}
	}
}

void AGameMode_MainLevel::FH_Logout(APlayerController_FH* PlayerController_Fh)
{
	if (PlayerController_Fh)
	{
		Fh_MapPlayerController.Remove(PlayerController_Fh->GetUniqueID());
		PlayerController_Fh->FH_ClearUpdateLocationTimer();
	}
}
