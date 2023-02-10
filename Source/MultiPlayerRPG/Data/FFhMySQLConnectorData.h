#pragma once
#include "FFhMySQLConnectorData.generated.h"

USTRUCT(BlueprintType)
struct FFhMySQLConnectorData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Data|MySQL")
	FString Host{"47.101.140.198"};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Data|MySQL")
	FString UserName{"root"};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Data|MySQL")
	FString PassWord{"752972182"};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Data|MySQL")
	FString DBName{"MultiPlayerRPG"};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Data|MySQL")
	int32 Port{7706};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Data|MySQL")
	FString ConnectMessage{""};
};