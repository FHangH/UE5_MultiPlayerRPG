#pragma once
#include "FHColorEnum.generated.h"

UENUM(BlueprintType)
enum class EFH_Color : uint8
{
	EFHC_WHITE	UMETA(DisplayName="White"),
	EFHC_BLUE	UMETA(DisplayName="Blue"),
	EFHC_RED	UMETA(DisplayName="Red"),
	EFHC_YELLOW	UMETA(DisplayName="Yellow"),
	EFHC_GREEN	UMETA(DisplayName="Green"),
	EFHC_BLACK	UMETA(DisplayName="Black"),
	EFHC_PINK	UMETA(DisplayName="Pink"),
	EFHC_GOLD	UMETA(DisplayName="Gold"),
	EFHC_SILVER	UMETA(DisplayName="Silver"),
};

namespace EColor
{
	static EFH_Color FH_SwitchByString(const FString& InString)
	{
		if (InString == "WHITE"){return EFH_Color::EFHC_WHITE;}
		if (InString == "BLUE"){return EFH_Color::EFHC_BLUE;}
		if (InString == "RED"){return EFH_Color::EFHC_RED;}
		if (InString == "YELLOW"){return EFH_Color::EFHC_YELLOW;}
		if (InString == "GREEN"){return EFH_Color::EFHC_GREEN;}
		if (InString == "BLACK"){return EFH_Color::EFHC_BLACK;}
		if (InString == "PINK"){return EFH_Color::EFHC_PINK;}
		if (InString == "GOLD"){return EFH_Color::EFHC_GOLD;}
		if (InString == "SILVER"){return EFH_Color::EFHC_SILVER;}
		return EFH_Color::EFHC_WHITE;
	}
}