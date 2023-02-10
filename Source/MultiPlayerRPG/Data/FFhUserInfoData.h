#pragma once
#include "FFhUserInfoData.generated.h"

USTRUCT(BlueprintType)
struct FFhUserInfoData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Data|UserInfo")
	FString fh_UserID{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Data|UserInfo")
	FString fh_UserName{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Data|UserInfo")
	FString fh_UserEmail{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Data|UserInfo")
	FString fh_UserPassWord{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Data|UserInfo")
	bool fh_UserIsLogin{false};
};