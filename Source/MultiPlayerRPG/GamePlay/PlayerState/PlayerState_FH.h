#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "MultiPlayerRPG/Data/FFhUserInfoData.h"
#include "MultiPlayerRPG/Data/FFhPlayerInfoData.h"
#include "MultiPlayerRPG/Data/FFhWeaponInfoData.h"
#include "MultiPlayerRPG/Data/FFhPropsInfoData.h"
#include "PlayerState_FH.generated.h"

struct FFhUserInfoData;
struct FFhPlayerInfoData;
struct FFhWeaponInfoData;
struct FFhPropsInfoData;
class APlayerController_FH;

UCLASS()
class MULTIPLAYERRPG_API APlayerState_FH : public APlayerState
{
	GENERATED_BODY()

protected:
	APlayerState_FH();
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;
	
/* My Code */
public:
	// Property
	UPROPERTY(Replicated, BlueprintReadOnly, Category="FH|PlayerState|UserData")
	FFhUserInfoData Fh_UserInfoData;
	UPROPERTY(Replicated, BlueprintReadOnly, Category="FH|PlayerState|PlayerData")
	FFhPlayerInfoData Fh_PlayerInfoData;
	UPROPERTY(Replicated, BlueprintReadOnly, Category="FH|PlayerState|SwordData")
	FFhWeaponInfoData Fh_WeaponInfoFirstSlotData;
	UPROPERTY(Replicated, BlueprintReadOnly, Category="FH|PlayerState|GunData")
	FFhWeaponInfoData Fh_WeaponInfoSecondSlotData;
	UPROPERTY(Replicated, BlueprintReadOnly, Category="FH|PlayerState|PropsData")
	FFhPropsInfoData Fh_PropsInfoData;
};