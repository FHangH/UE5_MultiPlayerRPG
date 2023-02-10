#pragma once
#include "MultiPlayerRPG/GlobalVariable/FHWeaponQuality.h"
#include "MultiPlayerRPG/GlobalVariable/FHWeaponTypeEnum.h"
#include "FFhWeaponInfoData.generated.h"

USTRUCT(BlueprintType)
struct FFhWeaponInfoData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Data|WeaponInfo")
	FString fh_WeaponID{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Data|WeaponInfo")
	FString fh_UserID{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Data|WeaponInfo")
	EFH_WeaponQuality fh_WeaponQuality{EFH_WeaponQuality::EFHWQ_C};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Data|WeaponInfo")
	float fh_WeaponDamage{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Data|WeaponInfo")
	float fh_WeaponCritRate{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Data|WeaponInfo")
	float fh_WeaponCritMultiplier{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Data|WeaponInfo")
	EFH_WeaponType fh_WeaponType{EFH_WeaponType::EFHWT_SWORD};
};

