#pragma once

#include "CoreMinimal.h"
#include "BPFuncLib_FHSQL.h"
#include "GameFramework/PlayerController.h"
#include "MultiPlayerRPG/Data/FFhUserInfoData.h"
#include "PlayerController_Start.generated.h"

const FString USER_SAVEGAME = TEXT("UserSaveGame");

class UGameInstanceRPG;
struct FFhUserInfoData;
struct FResArray;
class UUserSaveGame;
class APlayerState_FH;

UCLASS()
class MULTIPLAYERRPG_API APlayerController_Start : public APlayerController
{
	GENERATED_BODY()

protected:
	APlayerController_Start();
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;
	
/* My Code */
	// Property
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="FH|CameInstance", meta=(AllowPrivateAccess=true))
	UGameInstanceRPG* Fh_GameInstance;
	UPROPERTY()
	APlayerState_FH* Fh_PlayerState;
	UPROPERTY()
	UUserSaveGame* SaveGameInstance;

	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess=true))
	FFhUserInfoData Fh_UserInfoData;
	UPROPERTY()
	FResArray Fh_UserInfoArray;
	UPROPERTY()
	FResArray Fh_WeaponIDSetArray;
	
	// Function
public:
	// Get
	UFUNCTION(Category="FH|PlayerController")
	FString FH_GetUserIDFromTable();
	
	UFUNCTION()
	void FH_OnCreateUIStart();
	
	UFUNCTION()
	void FH_SetInputModeOnlyUI(const bool IsShowMouse);

	UFUNCTION(BlueprintPure, Category="FH|PlayerController")
	bool FH_CheckPutLoginInfo(const FString Name, const FString Password);
	UFUNCTION()
	bool FH_CheckLoginUserIsGaming(UFH_ConnectionObject* Connector, const FString Name, const FString Password);
	
	UFUNCTION(BlueprintPure, Category="FH|PlayerController")
	bool FH_CheckPutRegInfo(const FString Name, const FString Email, const FString Password);
	UFUNCTION()
	bool FH_CheckRegUserNameAndEmailIsExisted(UFH_ConnectionObject* Connector, const FString Name, const FString Email);

	UFUNCTION(BlueprintCallable, Category="FH|PlayerContrtoller")
	void FH_ClientTravelToMap(const FName URL);

	UFUNCTION(Category="FH|PlayerController")
	void FH_PreInitUserInfo(const FString Name, const FString Password);
	
	UFUNCTION(BlueprintCallable, Category="FH|PlayerController")
	bool FH_InitUserInfo();

	UFUNCTION(Category="FH|PlayerController")
	void FH_InitUserInfoData();

	UFUNCTION(Category="FH|PlayerController")
	void FH_CreateSaveGame();
	
	UFUNCTION(Category="FH|PlayerController")
	void FH_UserInfoToSaveGame();

	UFUNCTION(Category="FH|PlayerController")
	void FH_SendUserInfoToGameInstance();
	
	UFUNCTION(Category="FH|PlayerController")
	bool FH_InsertAndUpdateDefaultInfoToMySQL(const FString Name);

	UFUNCTION(Category="FH|PlayerController")
	bool FH_InsertDefaultInfoToPlayerInfoTable(const FString Name);

	UFUNCTION(Category="FH|PlayerController")
	bool FH_InsertDefaultInfoToWeaponInfoTable();

	UFUNCTION(Category="FH|PlayerController")
	bool FH_SelectWeaponIDSetByUserID();

	UFUNCTION(Category="FH|PlayerController")
	FString FH_WeaponIDSetToString();

	UFUNCTION(Category="FH|PlayerController")
	bool FH_UpdateWeaponIDSetToPlayerInfoTable();

	UFUNCTION(Category="FH|PlayerController")
	bool FH_InsertDefaultInfoToPropsInfoTable(const FString Name);
};