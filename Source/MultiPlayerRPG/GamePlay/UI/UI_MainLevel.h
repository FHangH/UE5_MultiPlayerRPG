#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MultiPlayerRPG/GamePlay/PlayerController/PlayerController_FH.h"
#include "UI_MainLevel.generated.h"

class UUI_ShowInfo;
class APlayerController_FH;

UCLASS()
class MULTIPLAYERRPG_API UUI_MainLevel : public UUserWidget
{
	GENERATED_BODY()

/* My Code */	
	// Property
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="FH|UI", meta=(AllowPrivateAccess=true))
	UUI_ShowInfo* Fh_UI_ShowInfo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="FH|UI")
	TSubclassOf<UUI_ShowInfo> Fh_UI_ShowInfoClass;
	
	// Function
public:
	UFUNCTION(BlueprintImplementableEvent)
	void FH_SetText_UserInfo_UserID(const FString& ID);

	UFUNCTION(BlueprintImplementableEvent)
	void FH_SetText_PlayerInfo_Grade(const int Grade);
	
	UFUNCTION(BlueprintImplementableEvent)
	void FH_SetText_PlayerInfo_CurHP_MaxHP(const float CurHP, const float MaxHP);

	UFUNCTION(BlueprintImplementableEvent)
	void FH_SetUI_ShowUI(const bool IsShow);
	UFUNCTION(BlueprintImplementableEvent)
	void FH_SetUI_UpdateShowUI(
		const FFhUserInfoData& UserInfoData, const FFhPlayerInfoData& PlayerInfoData,
		const FFhWeaponInfoData& FirstSlot, const FFhWeaponInfoData& SecondSlot, const FFhPropsInfoData& PropsInfoData);

	UFUNCTION(BlueprintImplementableEvent)
	void FH_GetPlayerController(const APlayerController_FH* PlayerController_Fh);
};