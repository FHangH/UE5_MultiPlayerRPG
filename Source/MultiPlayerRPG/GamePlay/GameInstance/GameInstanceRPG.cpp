#include "GameInstanceRPG.h"
#include "BPFuncLib_FHSQL.h"
#include "Blueprint/UserWidget.h"
#include "MultiPlayerRPG/GamePlay/UI/UI_Start.h"

void UGameInstanceRPG::Init()
{
	Super::Init();
}

void UGameInstanceRPG::Shutdown()
{
	Super::Shutdown();
}

void UGameInstanceRPG::StartGameInstance()
{
	Super::StartGameInstance();

	FH_InitMySQLConnector();
}

void UGameInstanceRPG::BeginDestroy()
{
	if (Fh_MySQLConnector)
	{
		if (UBPFuncLib_FHSQL::GetConnectionState(Fh_MySQLConnector))
		{
			UBPFuncLib_FHSQL::CloseConnection(Fh_MySQLConnector);
		}
	}
	Super::BeginDestroy();
}

/* My Code */
void UGameInstanceRPG::FH_OnCreateUIStart()
{
	if (IsValid(Fh_UI_StartClass))
	{
		Fh_UI_Start = CreateWidget<UUI_Start>(GetWorld(), Fh_UI_StartClass);
	}
	FH_InitMySQLConnector();
}

void UGameInstanceRPG::FH_InitMySQLConnector()
{
	Fh_MySQLConnector =
		Fh_MySQLConnector == nullptr ?
			UBPFuncLib_FHSQL::ConnectToMySQL(
				Fh_MySQLConnectorData.Host,
				Fh_MySQLConnectorData.UserName,
				Fh_MySQLConnectorData.PassWord,
				Fh_MySQLConnectorData.DBName,
				Fh_MySQLConnectorData.Port,
				fh_ConnectMessage) : Fh_MySQLConnector;
}

void UGameInstanceRPG::FH_InitMySQLConnectorData(FFhMySQLConnectorData MySQLConnectorData)
{
	Fh_MySQLConnectorData.Host = MySQLConnectorData.Host;
	Fh_MySQLConnectorData.UserName = MySQLConnectorData.UserName;
	Fh_MySQLConnectorData.PassWord = MySQLConnectorData.PassWord;
	Fh_MySQLConnectorData.Port = MySQLConnectorData.Port;
	Fh_MySQLConnectorData.ConnectMessage = MySQLConnectorData.ConnectMessage;
}