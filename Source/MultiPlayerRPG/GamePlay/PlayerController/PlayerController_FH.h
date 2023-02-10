#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MultiPlayerRPG/Utils/DelegatePlayerControllerFH.h"
#include "MultiPlayerRPG/Data/FFhUserInfoData.h"
#include "MultiPlayerRPG/Data/FFhPlayerInfoData.h"
#include "MultiPlayerRPG/Data/FFhWeaponInfoData.h"
#include "MultiPlayerRPG/Data/FFhPropsInfoData.h"
#include "BPFuncLib_FHSQL.h"
#include "PlayerController_FH.generated.h"

class APlayerState_FH;
struct FFhUserInfoData;
struct FFhPlayerInfoData;
struct FFhWeaponInfoData;
struct FFhPropsInfoData;
struct FResArray;
struct FTimerHandle;
class UGameInstanceRPG;
class AGameMode_MainLevel;
class UUI_MainLevel;
class UUI_Waiting;
class ACharacter_FH;

UCLASS()
class MULTIPLAYERRPG_API APlayerController_FH : public APlayerController
{
	GENERATED_BODY()

protected:
	APlayerController_FH();
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

/* My Code */
	// Property
public:
	/*
	 * PlayerController Index
	 */
	UPROPERTY(Replicated)
	int PCIndex;
	
	/*
	 * UserInfo PlayerInfo WeaponInfo PropsInfo
	 */
	UPROPERTY(VisibleAnywhere, Replicated, BlueprintReadWrite, Category="FH|PlayerController|UserData", meta=(AllowPrivateAccess=true))
	FFhUserInfoData Fh_UserInfoData;
	UPROPERTY(VisibleAnywhere, Replicated, BlueprintReadWrite, Category="FH|PlayerController|PlayerData", meta=(AllowPrivateAccess=true))
	FFhPlayerInfoData Fh_PlayerInfoData;
	UPROPERTY(VisibleAnywhere, Replicated, BlueprintReadWrite, Category="FH|PlayerController|SwordData", meta=(AllowPrivateAccess=true))
	FFhWeaponInfoData Fh_WeaponInfoFirstSlotData;
	UPROPERTY(VisibleAnywhere, Replicated, BlueprintReadWrite, Category="FH|PlayerController|GunData", meta=(AllowPrivateAccess=true))
	FFhWeaponInfoData Fh_WeaponInfoSecondSlotData;
	UPROPERTY(VisibleAnywhere, Replicated, BlueprintReadWrite, Category="FH|PlayerController|GunData", meta=(AllowPrivateAccess=true))
	FFhPropsInfoData Fh_PropsInfoData;
	UPROPERTY(VisibleAnywhere, Replicated)
	FResArray Fh_UserInfoArray;
	UPROPERTY(VisibleAnywhere, Replicated)
	FResArray Fh_PlayerInfoArray;
	UPROPERTY(VisibleAnywhere, Replicated)
	FResArray Fh_WeaponInfoArray;
	UPROPERTY(VisibleAnywhere, Replicated)
	FResArray Fh_PropsInfoArray;

	/*
	 * UI_Main, UI_DelayWaiting, UI_ShowInfo
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="FH|PlayerController|UI", meta=(AllowPrivateAccess=true))
	UUI_MainLevel* Fh_UI_Main;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="FH|PlayerController|UI")
	TSubclassOf<UUI_MainLevel> Fh_UI_MainClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="FH|PlayerController|UI", meta=(AllowPrivateAccess=true))
	UUI_Waiting* Fh_UI_DelayWaiting;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="FH|PlayerController|UI")
	TSubclassOf<UUI_Waiting> Fh_UI_DelayWaitingClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="FH|PlayerController|UI")
	bool fh_IsTableKeyPressed{false};

	/*
	 * GamePlay
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="FH|CameInstance", meta=(AllowPrivateAccess=true))
	UGameInstanceRPG* Fh_GameInstance;

	UPROPERTY(EditAnywhere, Category="FH|PlayerCharacter", meta=(AllowPrivateAccess=true))
	TSubclassOf<ACharacter_FH> Fh_CharacterClass;
	UPROPERTY()
	ACharacter_FH* Fh_Player;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="FH|GameMode", meta=(AllowPrivateAccess=true))
	AGameMode_MainLevel* Fh_GameMode;
	
	/*
	 * Delegate
	 */
	FHDelegateFHMoveForward FHDG_MoveForward;
	FHDelegateFHMoveRight FHDG_MoveRight;
	FHDelegateFHPitchCamera FHDG_PitchCamera;
	FHDelegateFHYawCamera FHDG_YawCamera;
	FHDelegateFHJump FHDG_Jump;
	FHDelegateFHStopJumping FHDG_StopJumping;

	/*
	 * Character All Game Values
	 */
	// Character Input
	UPROPERTY()
	bool fh_IsCharacterInput{true};

	// Character All Game Values => Server (No Replicates Functions In HasAuthority Or ServerRPC)
	/*UPROPERTY(ReplicatedUsing=FH_OnRepPlayerCurGrade, Category="FH|PlayerCharacter|GameValues")
	int PlayerCurGrade;
	int PlayerMaxGrade;
	float PlayerCurHP;
	float PlayerMaxHP;
	float PlayerCurExp;
	float PlayerCurNeedExp;
	int PlayerPropsBNum;
	int PlayerPropsANum;
	int PlayerPropsSNum;
	int CToBNeedPropsBCount;
	int BToANeedPropsACount;
	int AToSNeedPropsSCount;*/
	
	// Function
public:
	/*
	 * Get
	 */
	UFUNCTION()
	APlayerState_FH* FH_GetPlayerState() const;
	UFUNCTION()
	void FH_GetUserInfoFromGameInstance();
	UFUNCTION()
	bool FH_SelectInfoFromTable(const FString& TableName, FResArray& ResInfoArray);
	UFUNCTION()
	void FH_UpdateUserLoginStatusToMySQL(UFH_ConnectionObject* Connector, const bool IsLogin);
	UFUNCTION()
	bool FH_GetUserInfoFromMySQL();
	UFUNCTION()
	bool FH_GetPlayerInfoFromMySQl();
	UFUNCTION()
	bool FH_GetWeaponInfoFromMySQL();
	UFUNCTION()
	bool FH_GetPropsInfoFromMySQL();

	/*
	 * Set
	 */
	UFUNCTION()
	void FH_SetInputModeOnlyGame(const bool IsShowMouse);
	UFUNCTION()
	void FH_SetInputModeGameAndUI(const bool IsShowMouse);
	UFUNCTION()
	void FH_SetPlayerControllerInput(const bool IsReceiveInput);
	UFUNCTION()
	void FH_SetCharacterInput(const bool IsReceiveInput);
	UFUNCTION(BlueprintCallable, Category="FH|PlayerController")
	void FH_SetCharacterColor(EFH_Color Color);
	UFUNCTION(Server, Reliable)
	void FH_ServerSetCharacterColor(EFH_Color Color);

	/**
	 * Init UserInfo, PlayerInfo, WeaponInfo
	 * Init PlayerState
	 */
	UFUNCTION()
	void FH_Init();
	UFUNCTION(Client, Reliable)
	void FH_ClientInit();
	UFUNCTION()
	void FH_InitUserInfo();
	UFUNCTION(Server, Reliable)
	void FH_ServerInitUserInfo(const FFhUserInfoData& UserInfoData);
	UFUNCTION()
	void FH_InitPlayerInfo();
	UFUNCTION(Server, Reliable)
	void FH_ServerInitPlayerInfo(const FFhPlayerInfoData& PlayerInfoData);
	UFUNCTION()
	void FH_InitWeaponInfo();
	UFUNCTION()
	void FH_InitWeaponSlotInfo(FFhWeaponInfoData& WeaponInfoData, const int32 Index);
	UFUNCTION(Server, Reliable)
	void FH_ServerInitWeaponInfo(const FFhWeaponInfoData& FirstSlot, const FFhWeaponInfoData& SecondSlot);
	UFUNCTION()
	void FH_InitPropsInfo();
	UFUNCTION(Server, Reliable)
	void FH_ServerInitPropsInfo(const FFhPropsInfoData& PropsInfoData);

	UFUNCTION()
	void FH_InitPlayerStateTableData(const FFhUserInfoData& UserInfoData, const FFhPlayerInfoData& PlayerInfoData,
		const FFhWeaponInfoData& FirstSlot, const FFhWeaponInfoData& SecondSlot, const FFhPropsInfoData& PropsInfoData);
	
	/*
	 * Delegate Input Movement
	 */
	UFUNCTION() void FH_MoveForward(const float Value);
	UFUNCTION() void FH_MoveRight(const float Value);
	UFUNCTION() void FH_PitchCamera(const float Value);
	UFUNCTION() void FH_YawCamera(const float Value);
	UFUNCTION() void FH_Jump();
	UFUNCTION() void FH_StopJumping();

	/*
	 * Input Key
	 */
	UFUNCTION() void FH_TableKey();

	/*
	 * UI_Main Init And Update
	 */
	UFUNCTION(Client, Reliable)
	void FH_ClientCreateUI_Main();
	UFUNCTION(Client, Reliable)
	void FH_ClientCreateWaitingDelayUI();
	UFUNCTION(Client, Reliable)
	void FH_ClientRemoveWaitingDelayUI();
	UFUNCTION()
	void FH_UpdateUI_ShowInfo();

	/*
	 * Travel Map
	 */
	UFUNCTION(BlueprintCallable, Category="FH|PlayerController")
	void FH_SeamlessTravelToMap(const FString URL);

	UFUNCTION(BlueprintCallable, Category="FH|PlayerContrtoller")
	void FH_ClientTravelToMap();

	/*
	 * Create Player At Location And Init Color From MySQL 
	 */
	UFUNCTION()
	void FH_DelaySpawnPlayer();
	UFUNCTION()
	void FH_SpawnPlayerAtLocation();

	/*
	 * Async Update Player Location To MySQL
	 */
	UPROPERTY()
	FTimerHandle Fh_UpdateLocationTimerHandle;
	UFUNCTION()
	void FH_StartUpdateLocationTimer();
	UFUNCTION()
	void FH_OnUpdateLocationTimer();
	UFUNCTION()
	void FH_ClearUpdateLocationTimer();
	UFUNCTION()
	void FH_UpdatePlayerLocationToMySQL();
	UFUNCTION(Client, Reliable)
	void FH_ClientUpdateLocationToMySQL();

	/*
	 * Update PlayerController UserInfo, PlayerInfo, WeaponInfo, PropsInfo
	 * Server RPC
	 */
	// UserInfo
	UFUNCTION(BlueprintCallable, Category="FH|PlayerController|UpdateInfoData")
	void FH_UpdateUserInfo(const FFhUserInfoData& UserInfoData);
	UFUNCTION(Server, Reliable)
	void FH_ServerUpdateUserInfo(const FFhUserInfoData& UserInfoData);

	// PlayerInfo
	UFUNCTION(BlueprintCallable, Category="FH|PlayerController|UpdateInfoData")
	void FH_UpdatePlayerInfo(const FFhPlayerInfoData& PlayerInfoData);
	UFUNCTION(Server, Reliable)
	void FH_ServerUpdatePlayerInfo(const FFhPlayerInfoData& PlayerInfoData);

	// WeaponInfo
	UFUNCTION(BlueprintCallable, Category="FH|PlayerController|UpdateInfoData")
	void FH_UpdateWeaponInfo(const FFhWeaponInfoData& FirstSlot, const FFhWeaponInfoData& SecondSlot);
	UFUNCTION(Server, Reliable)
	void FH_ServerUpdateWeaponInfo(const FFhWeaponInfoData& FirstSlot, const FFhWeaponInfoData& SecondSlot);

	// PropsInfo
	UFUNCTION(BlueprintCallable, Category="FH|PlayerController|UpdateInfoData")
	void FH_UpdatePropsInfo(const FFhPropsInfoData& PropsInfoData);
	UFUNCTION(Server, Reliable)
	void FH_ServerUpdatePropsInfo(const FFhPropsInfoData& PropsInfoData);

	/*
	 * Init And Update PlayerCharacter All Game Values And Call Back OnRep
	 */

	/*
	 * Back Login UI And Quit Game
	 */
	UFUNCTION(BlueprintCallable, Category="FH|PlayerController|Quit")
	void FH_BackLoginUI();
	UFUNCTION(Server, Reliable)
	void FH_ServerBackLoginUI();

	UFUNCTION(BlueprintCallable, Category="FH|PlayerController|Quit")
	void FH_QuitGame();
	UFUNCTION(Server, Reliable)
	void FH_ServerQuitGame();
};
