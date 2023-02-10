#include "PlayerController_Start.h"
#include "Kismet/GameplayStatics.h"
#include "MultiPlayerRPG/GamePlay/GameInstance/GameInstanceRPG.h"
#include "MultiPlayerRPG/GamePlay/UI/UI_Start.h"
#include "MultiPlayerRPG/GlobalVariable/FHWeaponTypeEnum.h"
#include "MultiPlayerRPG/SaveGame/UserSaveGame.h"
#include "MultiPlayerRPG/Utils/MySqlUtil.h"

APlayerController_Start::APlayerController_Start()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
}

void APlayerController_Start::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocalPlayerController())
	{
		Fh_GameInstance = Cast<UGameInstanceRPG>(GetGameInstance());
		FH_OnCreateUIStart();
	}
}

void APlayerController_Start::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

/* My Code */
FString APlayerController_Start::FH_GetUserIDFromTable()
{
	Fh_GameInstance = Fh_GameInstance == nullptr ? Cast<UGameInstanceRPG>(GetGameInstance()) : Fh_GameInstance;
	if (Fh_GameInstance)
	{
		const auto Connector = Fh_GameInstance->FH_GetMySQLConnector();
		FString ResStrID;
		if (Connector)
		{
			UBPFuncLib_FHSQL::SelectOnTableGetWhatByCondition(
				Connector,
				"UserInfo",
				"UserID",
				"UserName",
				Fh_UserInfoData.fh_UserName,
				ResStrID);
		}
		Fh_UserInfoData.fh_UserID = ResStrID;
		return ResStrID;
	}
	return "";
}

void APlayerController_Start::FH_OnCreateUIStart()
{
	Fh_GameInstance = Fh_GameInstance == nullptr ? Cast<UGameInstanceRPG>(GetGameInstance()) : Fh_GameInstance;
	if (Fh_GameInstance)
	{
		Fh_GameInstance->FH_OnCreateUIStart();
		const auto UI = Fh_GameInstance->FH_GetUIStart();
		if (UI){UI->AddToViewport();}
	}
	FH_SetInputModeOnlyUI(true);
}

void APlayerController_Start::FH_SetInputModeOnlyUI(const bool IsShowMouse)
{
	FInputModeUIOnly InputModeUIOnly;
	InputModeUIOnly.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	SetInputMode(InputModeUIOnly);
	SetShowMouseCursor(IsShowMouse);
}

bool APlayerController_Start::FH_CheckPutLoginInfo(const FString Name, const FString Password)
{
	if (Name == "" || Password == ""){return false;}
	Fh_GameInstance = Fh_GameInstance == nullptr ? Cast<UGameInstanceRPG>(GetGameInstance()) : Fh_GameInstance;
	if (Fh_GameInstance)
	{
		FString ResStrPassword;
		const auto Connector = Fh_GameInstance->FH_GetMySQLConnector();
		if (Connector)
		{
			UBPFuncLib_FHSQL::
			SelectOnTableGetWhatByCondition(
			Connector, "UserInfo", "UserPassWord", "UserName", Name, ResStrPassword);
		}
		if (ResStrPassword == Password)
		{
			const auto IsGaming = FH_CheckLoginUserIsGaming(Connector, Name, Password);
			if (IsGaming){return false;}
			FH_PreInitUserInfo(Name, Password);
			return true;
		}
	}
	return false;
}

bool APlayerController_Start::FH_CheckLoginUserIsGaming(UFH_ConnectionObject* Connector, const FString Name,
	const FString Password)
{
	if (Connector)
	{
		FString UserGamingStatus{};
		const auto SqlQuery =
			MySqlUtil::SelectUserIsGamingByConditionSqlQuery(Name, Password);
		const auto IsSelect =
			UBPFuncLib_FHSQL::SelectOnTableGetWhatByMultiCondition(Connector, SqlQuery, UserGamingStatus);
		if (IsSelect)
		{
			const auto IsUserGaming = UserGamingStatus.ToBool();
			if (IsUserGaming)
			{
				return true;
			}
			return false;
		}
		return false;
	}
	return false;
}

bool APlayerController_Start::FH_CheckPutRegInfo(const FString Name, const FString Email, const FString Password)
{
	if (Name == "" || Email == "" || Password == ""){return false;}
	Fh_GameInstance = Fh_GameInstance == nullptr ? Cast<UGameInstanceRPG>(GetGameInstance()) : Fh_GameInstance;
	if (Fh_GameInstance)
	{
		const auto Connector = Fh_GameInstance->FH_GetMySQLConnector();
		if (Connector)
		{
			// Check Reg UserName And UserEmail Is Existed
			const auto IsNotExisted = FH_CheckRegUserNameAndEmailIsExisted(Connector, Name, Email);
			if (IsNotExisted)
			{
				// Insert MySQL
				auto SqlQuery = MySqlUtil::InsertIntoUserInfoSqlQuery(Name, Email, Password);
				SqlQuery = UBPFuncLib_FHSQL::InsertFormatSqlQuery("UserInfo", SqlQuery);
				const auto IsInsert = UBPFuncLib_FHSQL::ActionOnTableData(Connector, SqlQuery);
				if (IsInsert)
				{
					FH_PreInitUserInfo(Name, Password);
					return FH_InsertAndUpdateDefaultInfoToMySQL(Name);
				}
			}
			return false;
		}
	}
	return false;
}

bool APlayerController_Start::FH_CheckRegUserNameAndEmailIsExisted(UFH_ConnectionObject* Connector, const FString Name,
	const FString Email)
{
	if (Connector)
	{
		FString UserID;
		const auto SqlQuery =
			MySqlUtil::SelectExistedUserByConditionSqlQuery(Name, Email);
		const auto IsSelect =
			UBPFuncLib_FHSQL::SelectOnTableGetWhatByMultiCondition(Connector, SqlQuery, UserID);
		if (IsSelect)
		{
			if (UserID == "")
			{
				return true;
			}
			return false;
		}
		return false;
	}
	return false;
}

void APlayerController_Start::FH_ClientTravelToMap(const FName URL)
{
	ClientTravel(URL.ToString(), TRAVEL_Absolute);
}

void APlayerController_Start::FH_PreInitUserInfo(const FString Name, const FString Password)
{
	Fh_UserInfoData.fh_UserName = Name;
	Fh_UserInfoData.fh_UserPassWord = Password;
}

bool APlayerController_Start::FH_InitUserInfo()
{
	Fh_GameInstance = Fh_GameInstance == nullptr ? Cast<UGameInstanceRPG>(GetGameInstance()) : Fh_GameInstance;
	bool IsSelect{false};
	if (Fh_GameInstance)
	{
		const auto Connector = Fh_GameInstance->FH_GetMySQLConnector();
		const auto ID = FH_GetUserIDFromTable();
		if (Connector && ID != "")
		{
			const auto SqlQuery = MySqlUtil::SelectAllFromTableByConditionSqlQuery(
				"UserInfo",
				"UserID",
				ID);
			IsSelect = UBPFuncLib_FHSQL::SelectOnTableToMap(Connector, SqlQuery, Fh_UserInfoArray);
		}
	}
	if (IsSelect && Fh_UserInfoArray.ResArray.Num() != 0)
	{
		FH_InitUserInfoData();
		FH_CreateSaveGame();
		FH_UserInfoToSaveGame();
		FH_SendUserInfoToGameInstance();
		return true;
	}
	return false;
}

void APlayerController_Start::FH_InitUserInfoData()
{
	if (HasAuthority())
	{
		Fh_UserInfoData.fh_UserID = *Fh_UserInfoArray.ResArray[0].RowMap.Find("UserID");
		Fh_UserInfoData.fh_UserName = *Fh_UserInfoArray.ResArray[0].RowMap.Find("UserName");
		Fh_UserInfoData.fh_UserEmail = *Fh_UserInfoArray.ResArray[0].RowMap.Find("UserEmail");
		Fh_UserInfoData.fh_UserPassWord = *Fh_UserInfoArray.ResArray[0].RowMap.Find("UserPassWord");
	}
}

void APlayerController_Start::FH_CreateSaveGame()
{
	if (IsLocalPlayerController())
	{
		const auto IsExist = UGameplayStatics::DoesSaveGameExist(USER_SAVEGAME, 0);
		if (IsExist)
		{
			SaveGameInstance = Cast<UUserSaveGame>(UGameplayStatics::LoadGameFromSlot(USER_SAVEGAME, 0));
		}
		SaveGameInstance = Cast<UUserSaveGame>(UGameplayStatics::CreateSaveGameObject(UUserSaveGame::StaticClass()));
	}
}

void APlayerController_Start::FH_UserInfoToSaveGame()
{
	if (IsLocalPlayerController() && SaveGameInstance && !Fh_UserInfoArray.ResArray.IsEmpty())
	{
		SaveGameInstance->fh_UserID = *Fh_UserInfoArray.ResArray[0].RowMap.Find("UserID");
		SaveGameInstance->fh_UserName = *Fh_UserInfoArray.ResArray[0].RowMap.Find("UserName");
		SaveGameInstance->fh_UserEmail = *Fh_UserInfoArray.ResArray[0].RowMap.Find("UserEmail");
		SaveGameInstance->fh_UserPassWord = *Fh_UserInfoArray.ResArray[0].RowMap.Find("UserPassWord");
		UGameplayStatics::SaveGameToSlot(SaveGameInstance, USER_SAVEGAME, 0);
	}
}

void APlayerController_Start::FH_SendUserInfoToGameInstance()
{
	if (IsLocalPlayerController())
	{
		Fh_GameInstance = Fh_GameInstance == nullptr ? Cast<UGameInstanceRPG>(GetGameInstance()) : Fh_GameInstance;
		if (Fh_GameInstance)
		{
			Fh_GameInstance->FH_SetUserInfoData(Fh_UserInfoData);
		}
	}
}

bool APlayerController_Start::FH_InsertAndUpdateDefaultInfoToMySQL(const FString Name)
{
	if (FH_InsertDefaultInfoToPlayerInfoTable(Name))
	{
		if (FH_InsertDefaultInfoToWeaponInfoTable())
		{
			if (FH_UpdateWeaponIDSetToPlayerInfoTable())
			{
				return FH_InsertDefaultInfoToPropsInfoTable(Name);
			}
		}
	}
	return false;
}

bool APlayerController_Start::FH_InsertDefaultInfoToPlayerInfoTable(const FString Name)
{
	if (Name.IsEmpty()){return false;}
	FH_PreInitUserInfo(Name, Fh_UserInfoData.fh_UserPassWord);
	Fh_GameInstance = Fh_GameInstance == nullptr ? Cast<UGameInstanceRPG>(GetGameInstance()) : Fh_GameInstance;
	
	if (Fh_GameInstance)
	{
		const auto Connector = Fh_GameInstance->FH_GetMySQLConnector();
		const auto ID = FH_GetUserIDFromTable();
		if (Connector && ID != "")
		{
			const auto InsertValues = MySqlUtil::InsertIntoPlayerInfoDefaultDataSqlQuery(ID);
			const auto SqlQuery = UBPFuncLib_FHSQL::InsertFormatSqlQuery("PlayerInfo", InsertValues);
			return UBPFuncLib_FHSQL::ActionOnTableData(Connector, SqlQuery);
		}
	}
	return false;
}

bool APlayerController_Start::FH_InsertDefaultInfoToWeaponInfoTable()
{
	if (Fh_UserInfoData.fh_UserID.IsEmpty()){return false;}
	const FString ID = Fh_UserInfoData.fh_UserID;
	Fh_GameInstance = Fh_GameInstance == nullptr ? Cast<UGameInstanceRPG>(GetGameInstance()) : Fh_GameInstance;

	if (Fh_GameInstance)
	{
		const auto Connector = Fh_GameInstance->FH_GetMySQLConnector();
		if (Connector)
		{
			const auto InsertSwordValues = MySqlUtil::InsertIntoWeaponInfoDefaultDataSqlQuery(ID, WEAPONTYPE_SWORD);
			const auto SwordSqlQuery = UBPFuncLib_FHSQL::InsertFormatSqlQuery("WeaponInfo", InsertSwordValues);
			const auto InsertGunValues = MySqlUtil::InsertIntoWeaponInfoDefaultDataSqlQuery(ID, WEAPONTYPE_GUN);
			const auto GunSqlQuery = UBPFuncLib_FHSQL::InsertFormatSqlQuery("WeaponInfo", InsertGunValues);

			const auto IsSwordInsert = UBPFuncLib_FHSQL::ActionOnTableData(Connector, SwordSqlQuery);
			const auto IsGunInsert = UBPFuncLib_FHSQL::ActionOnTableData(Connector, GunSqlQuery);
			return IsSwordInsert && IsGunInsert;
		}
	}
	return false;
}

bool APlayerController_Start::FH_SelectWeaponIDSetByUserID()
{
	if (Fh_UserInfoData.fh_UserID.IsEmpty()){return false;}
	const FString ID = Fh_UserInfoData.fh_UserID;
	Fh_GameInstance = Fh_GameInstance == nullptr ? Cast<UGameInstanceRPG>(GetGameInstance()) : Fh_GameInstance;

	if (Fh_GameInstance)
	{
		const auto Connector = Fh_GameInstance->FH_GetMySQLConnector();
		if (Connector)
		{
			const FString SqlQuery =
				MySqlUtil::SelectWeaponIDSetFromWeaponInfoByConditionSqlQuery("UserID", ID);
			return UBPFuncLib_FHSQL::SelectOnTableToMap(Connector, SqlQuery, Fh_WeaponIDSetArray);
		}
	}
	return false;
}

FString APlayerController_Start::FH_WeaponIDSetToString()
{
	if (FH_SelectWeaponIDSetByUserID() && !Fh_WeaponIDSetArray.ResArray.IsEmpty())
	{
		const auto SwordWeaponID = *Fh_WeaponIDSetArray.ResArray[0].RowMap.Find("WeaponID");
		const auto GunWeaponID = *Fh_WeaponIDSetArray.ResArray[1].RowMap.Find("WeaponID");
		return FString::Printf(TEXT("(%s,%s)"), *SwordWeaponID, *GunWeaponID);
	}
	return "";
}

bool APlayerController_Start::FH_UpdateWeaponIDSetToPlayerInfoTable()
{
	if (Fh_UserInfoData.fh_UserID.IsEmpty()){return false;}
	const FString ID = Fh_UserInfoData.fh_UserID;
	const auto WeaponIDSetStr = FH_WeaponIDSetToString();
	
	if (WeaponIDSetStr == ""){return false;}
	Fh_GameInstance = Fh_GameInstance == nullptr ? Cast<UGameInstanceRPG>(GetGameInstance()) : Fh_GameInstance;

	if (Fh_GameInstance)
	{
		const auto Connector = Fh_GameInstance->FH_GetMySQLConnector();
		if (Connector)
		{
			const auto SetWhatAndValue = FString::Printf(TEXT("PlayerWeaponIDSet='%s'"), *WeaponIDSetStr);
			const auto Condition = FString::Printf(TEXT("UserID=%s"), *ID);
			const auto UpdateSqlQuery =
				MySqlUtil::UpdateTableSetColumnValueByConditionSqlQuery("PlayerInfo", SetWhatAndValue, Condition);
			return UBPFuncLib_FHSQL::ActionOnTableData(Connector, UpdateSqlQuery);
		}
	}
	return false;
}

bool APlayerController_Start::FH_InsertDefaultInfoToPropsInfoTable(const FString Name)
{
	if (Name.IsEmpty()){return false;}
	FH_PreInitUserInfo(Name, Fh_UserInfoData.fh_UserPassWord);
	Fh_GameInstance = Fh_GameInstance == nullptr ? Cast<UGameInstanceRPG>(GetGameInstance()) : Fh_GameInstance;
	
	if (Fh_GameInstance)
	{
		const auto Connector = Fh_GameInstance->FH_GetMySQLConnector();
		const auto ID = FH_GetUserIDFromTable();
		if (Connector && ID != "")
		{
			const auto InsertValues = MySqlUtil::InsertIntoPropsInfoDefaultDataSqlQuery(ID);
			const auto SqlQuery = UBPFuncLib_FHSQL::InsertFormatSqlQuery("PropsInfo", InsertValues);
			return UBPFuncLib_FHSQL::ActionOnTableData(Connector, SqlQuery);
		}
	}
	return false;
}
