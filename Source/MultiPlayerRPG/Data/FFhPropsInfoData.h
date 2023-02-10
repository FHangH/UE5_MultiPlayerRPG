#pragma once
#include "FFhPropsInfoData.generated.h"

USTRUCT(BlueprintType)
struct FFhPropsInfoData
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Data|PropsInfo")
	FString fh_UserID{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Data|PropsInfo")
	int fh_PropsB{0};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Data|PropsInfo")
	int fh_PropsA{0};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Data|PropsInfo")
	int fh_PropsS{0};
};