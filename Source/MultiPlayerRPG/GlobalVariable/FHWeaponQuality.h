#pragma once
#include "FHWeaponQuality.generated.h"

UENUM(BlueprintType)
enum class EFH_WeaponQuality : uint8
{
	EFHWQ_S		UMETA(DisplayName="S"),
	EFHWQ_A		UMETA(DisplayName="A"),
	EFHWQ_B		UMETA(DisplayName="B"),
	EFHWQ_C		UMETA(DisplayName="C")
};

namespace EWeaponQuality
{
	static EFH_WeaponQuality FH_SwitchByString(const FString& InString)
	{
		if (InString == "S"){return EFH_WeaponQuality::EFHWQ_S;}
		if (InString == "A"){return EFH_WeaponQuality::EFHWQ_A;}
		if (InString == "B"){return EFH_WeaponQuality::EFHWQ_B;}
		if (InString == "C"){return EFH_WeaponQuality::EFHWQ_C;}
		return EFH_WeaponQuality::EFHWQ_C;
	}
}