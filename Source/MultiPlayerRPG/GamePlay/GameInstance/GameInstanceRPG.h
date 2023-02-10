#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MultiPlayerRPG/Data/FFhMySQLConnectorData.h"
#include "MultiPlayerRPG/Data/FFhUserInfoData.h"
#include "GameInstanceRPG.generated.h"

class UUI_Start;
class UFH_ConnectionObject;
struct FFhMySQLConnectorData;
struct FFhUserInfoData;

UCLASS()
class MULTIPLAYERRPG_API UGameInstanceRPG : public UGameInstance
{
	GENERATED_BODY()

protected:
	virtual void Init() override;
	virtual void Shutdown() override;
	virtual void StartGameInstance() override;
	virtual void BeginDestroy() override;
	
/* My Code */
	// Property
private:
	UPROPERTY(EditAnywhere, Category="UI|Start")
	TSubclassOf<UUserWidget> Fh_UI_StartClass;
	UPROPERTY(BlueprintReadWrite, meta=(AllowPrivateAccess=true), Category="UI|Start")
	UUI_Start* Fh_UI_Start;

	UPROPERTY(BlueprintReadWrite, meta=(AllowPrivateAccess=true), Category="Utils|MySQL")
	UFH_ConnectionObject* Fh_MySQLConnector;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess=true), Category="Utils|MySQL")
	FString fh_ConnectMessage;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess=true), Category="Utils|MySQL")
	FFhMySQLConnectorData Fh_MySQLConnectorData;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess=true), Category="Utils|MySQL")
	FFhUserInfoData Fh_UserInfoData;
	
	// Function
public:
	UFUNCTION()
	void FH_OnCreateUIStart();

	UFUNCTION()
	void FH_InitMySQLConnector();

	UFUNCTION(BlueprintCallable)
	void FH_InitMySQLConnectorData(FFhMySQLConnectorData MySQLConnectorData);
	
	// Get
public:
	UFUNCTION() FORCEINLINE UUI_Start* FH_GetUIStart() const {return Fh_UI_Start;}
	UFUNCTION() FORCEINLINE UFH_ConnectionObject* FH_GetMySQLConnector() const {return Fh_MySQLConnector;}
	UFUNCTION() FORCEINLINE FFhMySQLConnectorData FH_GetMySQLConnectorData() const {return Fh_MySQLConnectorData;}
	UFUNCTION() FORCEINLINE FFhUserInfoData FH_GetUserInfoData() const {return Fh_UserInfoData;}

	// Set
	UFUNCTION() FORCEINLINE void FH_SetUserInfoData(const FFhUserInfoData& UserInfoData){Fh_UserInfoData = UserInfoData;}
};