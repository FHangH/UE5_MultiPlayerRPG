#include "PlayerController_FH.h"
#include "Kismet/KismetStringLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "MultiPlayerRPG/GamePlay/Character/Character_FH.h"
#include "MultiPlayerRPG/GamePlay/GameInstance/GameInstanceRPG.h"
#include "MultiPlayerRPG/GamePlay/GameMode/GameMode_MainLevel.h"
#include "MultiPlayerRPG/GamePlay/PlayerState/PlayerState_FH.h"
#include "MultiPlayerRPG/GamePlay/UI/UI_MainLevel.h"
#include "MultiPlayerRPG/GamePlay/UI/UI_ShowInfo.h"
#include "MultiPlayerRPG/GamePlay/UI/UI_Waiting.h"
#include "MultiPlayerRPG/GlobalVariable/TickTime.h"
#include "MultiPlayerRPG/Utils/MySqlUtil.h"
#include "Net/UnrealNetwork.h"
//#include "MultiPlayerRPG/Utils/AsyncFunction.h"

APlayerController_FH::APlayerController_FH()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	NetUpdateFrequency = TICKNUM_5;
	MinNetUpdateFrequency = TICKNUM_5;
}

void APlayerController_FH::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		Fh_GameMode = Fh_GameMode == nullptr ? GetWorld()->GetAuthGameMode<AGameMode_MainLevel>() : Fh_GameMode;
	}
	
	FH_SetInputModeOnlyGame(false);

	if (IsLocalPlayerController())
	{
		FH_SetPlayerControllerInput(false);
	}
}

void APlayerController_FH::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAxis(TEXT("MoveForward"), this, &ThisClass::FH_MoveForward);
	InputComponent->BindAxis(TEXT("MoveRight"), this, &ThisClass::FH_MoveRight);
	InputComponent->BindAxis(TEXT("PitchCamera"), this, &ThisClass::FH_PitchCamera);
	InputComponent->BindAxis(TEXT("YawCamera"), this, &ThisClass::FH_YawCamera);
	InputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &ThisClass::FH_Jump);
	InputComponent->BindAction(TEXT("Jump"), IE_Released, this, &ThisClass::FH_StopJumping);
	InputComponent->BindAction(TEXT("TableKey"), IE_Pressed, this, &ThisClass::FH_TableKey);
}

void APlayerController_FH::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, Fh_UserInfoData);
	DOREPLIFETIME(ThisClass, Fh_PlayerInfoData);
	DOREPLIFETIME(ThisClass, Fh_WeaponInfoFirstSlotData);
	DOREPLIFETIME(ThisClass, Fh_WeaponInfoSecondSlotData);
	DOREPLIFETIME(ThisClass, Fh_PropsInfoData);
	DOREPLIFETIME(ThisClass, Fh_UserInfoArray);
	DOREPLIFETIME(ThisClass, Fh_PlayerInfoArray);
	DOREPLIFETIME(ThisClass, Fh_WeaponInfoArray);
	DOREPLIFETIME(ThisClass, Fh_PropsInfoArray);
	DOREPLIFETIME(ThisClass, PCIndex);
}

/* My Code */
/*
 * Delegate Input Movement
 */
void APlayerController_FH::FH_MoveForward(const float Value)
{
	if (!fh_IsCharacterInput){return;}
	if (Value == 0.f){return;}
	if (FHDG_MoveForward.ExecuteIfBound(Value)){}
}

void APlayerController_FH::FH_MoveRight(const float Value)
{
	if (!fh_IsCharacterInput){return;}
	if (Value == 0.f){return;}
	if (FHDG_MoveRight.ExecuteIfBound(Value)){}
}

void APlayerController_FH::FH_PitchCamera(const float Value)
{
	if (!fh_IsCharacterInput){return;}
	if (Value == 0.f){return;}
	if (FHDG_PitchCamera.ExecuteIfBound(Value)){}
}

void APlayerController_FH::FH_YawCamera(const float Value)
{
	if (!fh_IsCharacterInput){return;}
	if (Value == 0.f){return;}
	if (FHDG_YawCamera.ExecuteIfBound(Value)){}
}

void APlayerController_FH::FH_Jump()
{
	if (!fh_IsCharacterInput){return;}
	if (FHDG_Jump.ExecuteIfBound()){}
}

void APlayerController_FH::FH_StopJumping()
{
	if (!fh_IsCharacterInput){return;}
	if (FHDG_StopJumping.ExecuteIfBound()){}
}

/*
 * Input Key
 */
void APlayerController_FH::FH_TableKey()
{
	if (Fh_UI_Main && fh_IsTableKeyPressed)
	{
		fh_IsTableKeyPressed = false;
		Fh_UI_Main->FH_SetUI_ShowUI(false);
		FH_SetCharacterInput(true);
		FH_SetInputModeOnlyGame(false);
		return;
	}
	if (Fh_UI_Main)
	{
		fh_IsTableKeyPressed = true;
		FH_UpdateUI_ShowInfo();
		Fh_UI_Main->FH_SetUI_ShowUI(true);
		FH_SetCharacterInput(false);
		FH_SetInputModeGameAndUI(true);
	}
}

/*
 * Get
 */
APlayerState_FH* APlayerController_FH::FH_GetPlayerState() const
{
	return GetPlayerState<APlayerState_FH>();
}

void APlayerController_FH::FH_GetUserInfoFromGameInstance()
{
	if (IsLocalPlayerController())
	{
		Fh_GameInstance = Fh_GameInstance == nullptr ? Cast<UGameInstanceRPG>(GetGameInstance()) : Fh_GameInstance;
		if (Fh_GameInstance)
		{
			Fh_UserInfoData = Fh_GameInstance->FH_GetUserInfoData();
			const auto Connector = Fh_GameInstance->FH_GetMySQLConnector();
			if (Connector)
			{
				FH_UpdateUserLoginStatusToMySQL(Connector, true);
			}
		}
	}
}

bool APlayerController_FH::FH_SelectInfoFromTable(const FString& TableName, FResArray& ResInfoArray)
{
	Fh_GameInstance = Fh_GameInstance == nullptr ? Cast<UGameInstanceRPG>(GetGameInstance()) : Fh_GameInstance;
	if (Fh_GameInstance)
	{
		const auto Connector = Fh_GameInstance->FH_GetMySQLConnector();
		if (Connector && Fh_UserInfoData.fh_UserID != "")
		{
			const auto SqlQuery = MySqlUtil::SelectAllFromTableByConditionSqlQuery(
				TableName,
				"UserID",
				Fh_UserInfoData.fh_UserID);
			return UBPFuncLib_FHSQL::SelectOnTableToMap(Connector, SqlQuery, ResInfoArray);
		}
		return false;
	}
	return false;
}

void APlayerController_FH::FH_UpdateUserLoginStatusToMySQL(UFH_ConnectionObject* Connector, const bool IsLogin)
{
	const auto LoginStatus = static_cast<int>(IsLogin);
	const auto UpdateLoginStatusSqlQuery =
					MySqlUtil::UpdateTableSetColumnValueByConditionSqlQuery(
						TEXT("UserInfo"),
						FString::Printf(TEXT("UserIsLogin=%d"), LoginStatus), 
						FString::Printf(TEXT("UserID=%s"), *Fh_UserInfoData.fh_UserID));
	UBPFuncLib_FHSQL::ActionOnTableData(Connector, UpdateLoginStatusSqlQuery);
}

bool APlayerController_FH::FH_GetUserInfoFromMySQL()
{
	return FH_SelectInfoFromTable("UserInfo", Fh_UserInfoArray);
}

bool APlayerController_FH::FH_GetPlayerInfoFromMySQl()
{
	return FH_SelectInfoFromTable("PlayerInfo", Fh_PlayerInfoArray);
}

bool APlayerController_FH::FH_GetWeaponInfoFromMySQL()
{
	return FH_SelectInfoFromTable("WeaponInfo", Fh_WeaponInfoArray);
}

bool APlayerController_FH::FH_GetPropsInfoFromMySQL()
{
	return FH_SelectInfoFromTable("PropsInfo", Fh_PropsInfoArray);
}

/*
 * Set
 */
void APlayerController_FH::FH_SetInputModeOnlyGame(const bool IsShowMouse)
{
	FInputModeGameOnly InputModeGameOnly;
	InputModeGameOnly.SetConsumeCaptureMouseDown(false);
	SetInputMode(InputModeGameOnly);
	SetShowMouseCursor(IsShowMouse);
}

void APlayerController_FH::FH_SetInputModeGameAndUI(const bool IsShowMouse)
{
	FInputModeGameAndUI InputModeGameAndUI;
	InputModeGameAndUI.SetHideCursorDuringCapture(false);
	InputModeGameAndUI.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	SetInputMode(InputModeGameAndUI);
	SetShowMouseCursor(IsShowMouse);
}

void APlayerController_FH::FH_SetPlayerControllerInput(const bool IsReceiveInput)
{
	if (IsReceiveInput)
	{
		EnableInput(this);
	}
	else
	{
		DisableInput(this);	
	}
}

void APlayerController_FH::FH_SetCharacterInput(const bool IsReceiveInput)
{
	fh_IsCharacterInput = IsReceiveInput;
}

void APlayerController_FH::FH_SetCharacterColor(EFH_Color Color)
{
	FH_ServerSetCharacterColor(Color);
}

void APlayerController_FH::FH_ServerSetCharacterColor_Implementation(EFH_Color Color)
{
	if (Fh_Player)
	{
		Fh_Player->FH_UpdateCharacterColor(Color);
	}
}

/*
 * Init
 */
void APlayerController_FH::FH_Init()
{
	FH_ClientInit();
	if (HasAuthority())
	{
		FH_InitPlayerStateTableData(
			Fh_UserInfoData, Fh_PlayerInfoData, Fh_WeaponInfoFirstSlotData, Fh_WeaponInfoSecondSlotData, Fh_PropsInfoData);
	}
	FH_ClientCreateUI_Main();
}

void APlayerController_FH::FH_ClientInit_Implementation()
{
	FH_GetUserInfoFromGameInstance();
	FH_InitUserInfo();
	FH_InitPlayerInfo();
	FH_InitWeaponInfo();
	FH_InitPropsInfo();
}

void APlayerController_FH::FH_InitUserInfo()
{
	if (FH_GetUserInfoFromMySQL() && Fh_UserInfoArray.ResArray.Num() != 0)
	{
		Fh_UserInfoData.fh_UserID = *Fh_UserInfoArray.ResArray[0].RowMap.Find("UserID");
		Fh_UserInfoData.fh_UserName = *Fh_UserInfoArray.ResArray[0].RowMap.Find("UserName");
		Fh_UserInfoData.fh_UserEmail = *Fh_UserInfoArray.ResArray[0].RowMap.Find("UserEmail");
		Fh_UserInfoData.fh_UserPassWord = *Fh_UserInfoArray.ResArray[0].RowMap.Find("UserPassWord");
		Fh_UserInfoData.fh_UserIsLogin = true;
	}
	FH_ServerInitUserInfo(Fh_UserInfoData);
}

void APlayerController_FH::FH_ServerInitUserInfo_Implementation(const FFhUserInfoData& UserInfoData)
{
	Fh_UserInfoData = UserInfoData;
}

void APlayerController_FH::FH_InitPlayerInfo()
{
	if (FH_GetPlayerInfoFromMySQl() && Fh_PlayerInfoArray.ResArray.Num() != 0)
	{
		// FString To FString
		Fh_PlayerInfoData.fh_UserID = *Fh_PlayerInfoArray.ResArray[0].RowMap.Find("UserID");
		// FString To Int
		Fh_PlayerInfoData.fh_PlayerGrade = FCString::Atoi(**Fh_PlayerInfoArray.ResArray[0].RowMap.Find("PlayerGrade"));
		// FString To Float
		Fh_PlayerInfoData.fh_PlayerMaxHealth = FCString::Atof(**Fh_PlayerInfoArray.ResArray[0].RowMap.Find("PlayerMaxHealth"));
		Fh_PlayerInfoData.fh_PlayerCurHealth = FCString::Atof(**Fh_PlayerInfoArray.ResArray[0].RowMap.Find("PlayerCurHealth"));
		Fh_PlayerInfoData.fh_PlayerDamage = FCString::Atof(**Fh_PlayerInfoArray.ResArray[0].RowMap.Find("PlayerDamage"));
		Fh_PlayerInfoData.fh_PlayerCritRate = FCString::Atof(**Fh_PlayerInfoArray.ResArray[0].RowMap.Find("PlayerCritRate"));
		Fh_PlayerInfoData.fh_PlayerCritMultiplier = FCString::Atof(**Fh_PlayerInfoArray.ResArray[0].RowMap.Find("PlayerCritMultiplier"));
		Fh_PlayerInfoData.fh_PlayerDefense = FCString::Atof(**Fh_PlayerInfoArray.ResArray[0].RowMap.Find("PlayerDefense"));
		// FString To FVector
		bool IsValid{true};
		UKismetStringLibrary::Conv_StringToVector(*Fh_PlayerInfoArray.ResArray[0].RowMap.Find("PlayerLocation"),
			Fh_PlayerInfoData.fh_PlayerLocation, IsValid);
		// FString To Enum
		Fh_PlayerInfoData.fh_PlayerColor = EColor::FH_SwitchByString(*Fh_PlayerInfoArray.ResArray[0].RowMap.Find("PlayerColor"));
		// FString To Float
		Fh_PlayerInfoData.fh_PlayerCurExp = FCString::Atof(**Fh_PlayerInfoArray.ResArray[0].RowMap.Find("PlayerCurExp"));
		Fh_PlayerInfoData.fh_PlayerNeedExp = FCString::Atof(**Fh_PlayerInfoArray.ResArray[0].RowMap.Find("PlayerNeedExp"));
		// FString To Int
		Fh_PlayerInfoData.fh_PlayerCurWeaponSlot = FCString::Atoi(**Fh_PlayerInfoArray.ResArray[0].RowMap.Find("PlayerCurWeaponSlot"));
		// FString To FString
		Fh_PlayerInfoData.fh_PlayerWeaponIDSet = *Fh_PlayerInfoArray.ResArray[0].RowMap.Find("PlayerWeaponIDSet");
	}
	FH_ServerInitPlayerInfo(Fh_PlayerInfoData);
}

void APlayerController_FH::FH_ServerInitPlayerInfo_Implementation(const FFhPlayerInfoData& PlayerInfoData)
{
	Fh_PlayerInfoData = PlayerInfoData;
}

void APlayerController_FH::FH_InitWeaponInfo()
{
	if (FH_GetWeaponInfoFromMySQL() && Fh_WeaponInfoArray.ResArray.Num() != 0)
	{
		FH_InitWeaponSlotInfo(Fh_WeaponInfoFirstSlotData, 0);
		FH_InitWeaponSlotInfo(Fh_WeaponInfoSecondSlotData, 1);
	}
	FH_ServerInitWeaponInfo(Fh_WeaponInfoFirstSlotData, Fh_WeaponInfoSecondSlotData);
}

void APlayerController_FH::FH_InitWeaponSlotInfo(FFhWeaponInfoData& WeaponInfoData, const int32 Index)
{
	WeaponInfoData.fh_WeaponID = *Fh_WeaponInfoArray.ResArray[Index].RowMap.Find("WeaponID");
	WeaponInfoData.fh_UserID = *Fh_WeaponInfoArray.ResArray[Index].RowMap.Find("UserID");
	WeaponInfoData.fh_WeaponQuality = EWeaponQuality::FH_SwitchByString(
		*Fh_WeaponInfoArray.ResArray[Index].RowMap.Find("WeaponQuality"));
	WeaponInfoData.fh_WeaponDamage = FCString::Atof(**Fh_WeaponInfoArray.ResArray[Index].RowMap.Find("WeaponDamage"));
	WeaponInfoData.fh_WeaponCritRate = FCString::Atof(**Fh_WeaponInfoArray.ResArray[Index].RowMap.Find("WeaponCritRate"));
	WeaponInfoData.fh_WeaponCritMultiplier = FCString::Atof(**Fh_WeaponInfoArray.ResArray[Index].RowMap.Find("WeaponCritMultiplier"));
	WeaponInfoData.fh_WeaponType = EWeaponType::FH_SwitchByString(
		*Fh_WeaponInfoArray.ResArray[Index].RowMap.Find("WeaponType"));
}

void APlayerController_FH::FH_ServerInitWeaponInfo_Implementation(const FFhWeaponInfoData& FirstSlot,
	const FFhWeaponInfoData& SecondSlot)
{
	Fh_WeaponInfoFirstSlotData = FirstSlot;
	Fh_WeaponInfoSecondSlotData = SecondSlot;
}

void APlayerController_FH::FH_InitPropsInfo()
{
	if (FH_GetPropsInfoFromMySQL() && Fh_PropsInfoArray.ResArray.Num() != 0)
	{
		Fh_PropsInfoData.fh_UserID = *Fh_PropsInfoArray.ResArray[0].RowMap.Find("UserID");
		Fh_PropsInfoData.fh_PropsB = FCString::Atoi(**Fh_PropsInfoArray.ResArray[0].RowMap.Find("PropsB"));
		Fh_PropsInfoData.fh_PropsA = FCString::Atoi(**Fh_PropsInfoArray.ResArray[0].RowMap.Find("PropsA"));
		Fh_PropsInfoData.fh_PropsS = FCString::Atoi(**Fh_PropsInfoArray.ResArray[0].RowMap.Find("PropsS"));
	}
	FH_ServerInitPropsInfo(Fh_PropsInfoData);
}

void APlayerController_FH::FH_ServerInitPropsInfo_Implementation(const FFhPropsInfoData& PropsInfoData)
{
	Fh_PropsInfoData = PropsInfoData;
}

void APlayerController_FH::FH_InitPlayerStateTableData(const FFhUserInfoData& UserInfoData,
	const FFhPlayerInfoData& PlayerInfoData,
	const FFhWeaponInfoData& FirstSlot, const FFhWeaponInfoData& SecondSlot, const FFhPropsInfoData& PropsInfoData)
{
	const auto PS = FH_GetPlayerState();
	if (PS)
	{
		PS->Fh_UserInfoData = UserInfoData;
		PS->Fh_PlayerInfoData = PlayerInfoData;
		PS->Fh_WeaponInfoFirstSlotData = FirstSlot;
		PS->Fh_WeaponInfoSecondSlotData = SecondSlot;
		PS->Fh_PropsInfoData = PropsInfoData;
	}
}

/*
 * UI_Main Init And Update
 */
void APlayerController_FH::FH_ClientCreateUI_Main_Implementation()
{
	if (IsValid(Fh_UI_MainClass) && GetWorld())
	{
		Fh_UI_Main = CreateWidget<UUI_MainLevel>(GetWorld(), Fh_UI_MainClass);
	}
	if (Fh_UI_Main)
	{
		Fh_UI_Main->AddToViewport();
		Fh_UI_Main->FH_SetText_UserInfo_UserID(Fh_UserInfoData.fh_UserID);
		Fh_UI_Main->FH_SetText_PlayerInfo_Grade(Fh_PlayerInfoData.fh_PlayerGrade);
		Fh_UI_Main->FH_SetText_PlayerInfo_CurHP_MaxHP(
			Fh_PlayerInfoData.fh_PlayerCurHealth, Fh_PlayerInfoData.fh_PlayerMaxHealth);
	}
}

void APlayerController_FH::FH_ClientCreateWaitingDelayUI_Implementation()
{
	if (IsValid(Fh_UI_DelayWaitingClass) && GetWorld())
	{
		Fh_UI_DelayWaiting = CreateWidget<UUI_Waiting>(GetWorld(), Fh_UI_DelayWaitingClass);
	}
	if (Fh_UI_DelayWaiting)
	{
		Fh_UI_DelayWaiting->AddToViewport();
	}
}

void APlayerController_FH::FH_ClientRemoveWaitingDelayUI_Implementation()
{
	if (Fh_UI_DelayWaiting)
	{
		Fh_UI_DelayWaiting->RemoveFromParent();
	}
	FH_SetPlayerControllerInput(true);
}

void APlayerController_FH::FH_UpdateUI_ShowInfo()
{
	if (IsLocalPlayerController() && Fh_UI_Main)
	{
		Fh_UI_Main->FH_SetUI_UpdateShowUI(
			Fh_UserInfoData, Fh_PlayerInfoData, Fh_WeaponInfoFirstSlotData, Fh_WeaponInfoSecondSlotData, Fh_PropsInfoData);
		Fh_UI_Main->FH_GetPlayerController(this);
	}
}

/*
 * Travel Map
 */
void APlayerController_FH::FH_SeamlessTravelToMap(const FString URL)
{
	ClientTravel(URL, TRAVEL_Relative, true);
}

void APlayerController_FH::FH_ClientTravelToMap()
{
	ClientTravel(TEXT("Map_Start"), TRAVEL_Absolute);
}

/*
 * Create Player
 */
void APlayerController_FH::FH_DelaySpawnPlayer()
{
	FH_ClientCreateWaitingDelayUI();
	const FLatentActionInfo LatentInfo
		(0, FMath::Rand(), TEXT("FH_SpawnPlayerAtLocation"), this);
	UKismetSystemLibrary::Delay(this, TICKTIME_3F, LatentInfo);
}

void APlayerController_FH::FH_SpawnPlayerAtLocation()
{
	if (GetWorld())
	{
		const FVector PlayerSpawnLocation =
			FVector(Fh_PlayerInfoData.fh_PlayerLocation.X, Fh_PlayerInfoData.fh_PlayerLocation.Y, 300.f);
		Fh_Player =
			GetWorld()->SpawnActor<ACharacter_FH>(
				Fh_CharacterClass, PlayerSpawnLocation, FRotator(0.f));
		if (Fh_Player)
		{
			FH_SetCharacterColor(Fh_PlayerInfoData.fh_PlayerColor);
			Fh_Player->SpawnDefaultController();
			Fh_Player->SetOwner(this);
			Possess(Fh_Player);
		}
	}
	FH_ClientRemoveWaitingDelayUI();
}

/*
* Async Update Player Location To MySQL
*/
void APlayerController_FH::FH_StartUpdateLocationTimer()
{
	if (GetWorld())
	{
		GetWorldTimerManager().SetTimer(
			Fh_UpdateLocationTimerHandle,
			this,
			&ThisClass::FH_OnUpdateLocationTimer,
			TICKTIME_3F,
			true,
			TICKTIME_3F);
	}
}

void APlayerController_FH::FH_OnUpdateLocationTimer()
{
	//UAsyncFunction::ExecuteAsyncFunction(this, &ThisClass::FH_UpdatePlayerLocationToMySQL);
	FH_UpdatePlayerLocationToMySQL();
}

void APlayerController_FH::FH_ClearUpdateLocationTimer()
{
	if (GetWorld())
	{
		GetWorldTimerManager().ClearTimer(Fh_UpdateLocationTimerHandle);
	}
}

void APlayerController_FH::FH_UpdatePlayerLocationToMySQL()
{
	FH_ClientUpdateLocationToMySQL();
}

void APlayerController_FH::FH_ClientUpdateLocationToMySQL_Implementation()
{
	Fh_GameInstance = Fh_GameInstance == nullptr ? Cast<UGameInstanceRPG>(GetGameInstance()) : Fh_GameInstance;
	if (!Fh_GameInstance){return;}
	
	const auto Connector = Fh_GameInstance->FH_GetMySQLConnector();
	if (!Connector){return;}

	const FVector LastLocation = GetPawn()->GetActorLocation();
	const auto UpdateSqlQuery = MySqlUtil::UpdateTableSetColumnValueByConditionSqlQuery(
			"PlayerInfo",
			FString::Printf(TEXT("PlayerLocation='%s'"), *LastLocation.ToString()),
			FString::Printf(TEXT("UserID=%s"), *Fh_PlayerInfoData.fh_UserID));
	UBPFuncLib_FHSQL::ActionOnTableData(Connector, UpdateSqlQuery);
}

/*
* Update PlayerController UserInfo, PlayerInfo, WeaponInfo, PropsInfo
* Server RPC
*/
// UserInfo
void APlayerController_FH::FH_UpdateUserInfo(const FFhUserInfoData& UserInfoData)
{
	FH_ServerUpdateUserInfo(UserInfoData);
}

void APlayerController_FH::FH_ServerUpdateUserInfo_Implementation(const FFhUserInfoData& UserInfoData)
{
	Fh_UserInfoData = UserInfoData;
}

// PlayerInfo
void APlayerController_FH::FH_UpdatePlayerInfo(const FFhPlayerInfoData& PlayerInfoData)
{
	FH_ServerUpdatePlayerInfo(PlayerInfoData);
}

void APlayerController_FH::FH_ServerUpdatePlayerInfo_Implementation(const FFhPlayerInfoData& PlayerInfoData)
{
	Fh_PlayerInfoData = PlayerInfoData;
}

// WeaponInfo
void APlayerController_FH::FH_UpdateWeaponInfo(const FFhWeaponInfoData& FirstSlot, const FFhWeaponInfoData& SecondSlot)
{
	FH_ServerUpdateWeaponInfo(FirstSlot, SecondSlot);
}

void APlayerController_FH::FH_ServerUpdateWeaponInfo_Implementation(const FFhWeaponInfoData& FirstSlot,
	const FFhWeaponInfoData& SecondSlot)
{
	Fh_WeaponInfoFirstSlotData = FirstSlot;
	Fh_WeaponInfoSecondSlotData = SecondSlot;
}

// PropsInfo
void APlayerController_FH::FH_UpdatePropsInfo(const FFhPropsInfoData& PropsInfoData)
{
	FH_ServerUpdatePropsInfo(PropsInfoData);
}

void APlayerController_FH::FH_ServerUpdatePropsInfo_Implementation(const FFhPropsInfoData& PropsInfoData)
{
	Fh_PropsInfoData = PropsInfoData;
}

/*
 * Init And Update PlayerCharacter All Game Values And Call Back OnRep
 */

/*
 * Back Login UI And Quit Game
 */
void APlayerController_FH::FH_BackLoginUI()
{
	FH_ServerBackLoginUI();
	FH_ClientTravelToMap();
}

void APlayerController_FH::FH_ServerBackLoginUI_Implementation()
{
	Fh_GameInstance = Fh_GameInstance == nullptr ? Cast<UGameInstanceRPG>(GetGameInstance()) : Fh_GameInstance;
	if (!Fh_GameInstance){return;}
	
	const auto Connector = Fh_GameInstance->FH_GetMySQLConnector();
	if (!Connector){return;}
	
	Fh_GameMode = Fh_GameMode == nullptr ? GetWorld()->GetAuthGameMode<AGameMode_MainLevel>() : Fh_GameMode;
	if (Fh_GameMode)
	{
		Fh_GameMode->FH_Logout(this);
		FH_UpdateUserLoginStatusToMySQL(Connector, false);
	}
}

void APlayerController_FH::FH_QuitGame()
{
	FH_ServerQuitGame();
	UKismetSystemLibrary::QuitGame(this, this, EQuitPreference::Quit, true);
}

void APlayerController_FH::FH_ServerQuitGame_Implementation()
{
	Fh_GameInstance = Fh_GameInstance == nullptr ? Cast<UGameInstanceRPG>(GetGameInstance()) : Fh_GameInstance;
	if (!Fh_GameInstance){return;}
	
	const auto Connector = Fh_GameInstance->FH_GetMySQLConnector();
	if (!Connector){return;}
	
	Fh_GameMode = Fh_GameMode == nullptr ? GetWorld()->GetAuthGameMode<AGameMode_MainLevel>() : Fh_GameMode;
	if (Fh_GameMode)
	{
		Fh_GameMode->FH_Logout(this);
		FH_UpdateUserLoginStatusToMySQL(Connector, false);
	}
}
