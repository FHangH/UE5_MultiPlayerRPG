#pragma once
#include "FHWeaponTypeEnum.generated.h"

const FString WEAPONTYPE_SWORD = TEXT("SWORD");
const FString WEAPONTYPE_GUN = TEXT("GUN");

UENUM(BlueprintType)
enum class EFH_WeaponType : uint8
{
	EFHWT_SWORD		UMETA(DisplayName="Sword"),
	EFHWT_GUN		UMETA(DisplayName="Gun")
};

namespace EWeaponType
{
	static EFH_WeaponType FH_SwitchByString(const FString& InString)
	{
		if (InString == WEAPONTYPE_SWORD){return EFH_WeaponType::EFHWT_SWORD;}
		if (InString == WEAPONTYPE_GUN){return EFH_WeaponType::EFHWT_GUN;}
		return EFH_WeaponType::EFHWT_SWORD;
	}
}