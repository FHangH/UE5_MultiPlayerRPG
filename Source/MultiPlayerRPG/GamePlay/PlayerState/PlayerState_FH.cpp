#include "PlayerState_FH.h"
#include "Net/UnrealNetwork.h"
#include "MultiPlayerRPG/GlobalVariable/TickTime.h"

APlayerState_FH::APlayerState_FH()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	NetUpdateFrequency = TICKNUM_5;
	MinNetUpdateFrequency = TICKNUM_5;
}

void APlayerState_FH::BeginPlay()
{
	Super::BeginPlay();
}

void APlayerState_FH::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ThisClass, Fh_UserInfoData);
	DOREPLIFETIME(ThisClass, Fh_PlayerInfoData);
	DOREPLIFETIME(ThisClass, Fh_WeaponInfoFirstSlotData);
	DOREPLIFETIME(ThisClass, Fh_WeaponInfoSecondSlotData);
	DOREPLIFETIME(ThisClass, Fh_PropsInfoData);
}