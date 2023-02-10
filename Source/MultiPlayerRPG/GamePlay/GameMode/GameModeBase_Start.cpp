#include "GameModeBase_Start.h"
#include "MultiPlayerRPG/GamePlay/GameState/GameState_FH.h"
#include "MultiPlayerRPG/GamePlay/PlayerController/PlayerController_Start.h"
#include "MultiPlayerRPG/GamePlay/PlayerState/PlayerState_FH.h"

AGameModeBase_Start::AGameModeBase_Start()
{
	PrimaryActorTick.bCanEverTick = false;
	bUseSeamlessTravel = true;
	PlayerControllerClass = APlayerController_Start::StaticClass();
	PlayerStateClass = APlayerState_FH::StaticClass();
}
