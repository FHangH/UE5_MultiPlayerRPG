#pragma once
#include "MultiPlayerRPG/GlobalVariable/FHColorEnum.h"
#include "FFhPlayerInfoData.generated.h"

USTRUCT(BlueprintType)
struct FFhPlayerInfoData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Data|PlayerInfo")
	FString fh_UserID{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Data|PlayerInfo")
	int32 fh_PlayerGrade{1};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Data|PlayerInfo")
	float fh_PlayerMaxHealth{100.f};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Data|PlayerInfo")
	float fh_PlayerCurHealth{100.f};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Data|PlayerInfo")
	float fh_PlayerDamage{1.f};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Data|PlayerInfo")
	float fh_PlayerCritRate{0.1f};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Data|PlayerInfo")
	float fh_PlayerCritMultiplier{1.1f};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Data|PlayerInfo")
	float fh_PlayerDefense{10.f};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Data|PlayerInfo")
	FVector fh_PlayerLocation{FVector(0.f)};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Data|PlayerInfo")
	EFH_Color fh_PlayerColor{EFH_Color::EFHC_WHITE};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Data|PlayerInfo")
	float fh_PlayerCurExp{0.f};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Data|PlayerInfo")
	float fh_PlayerNeedExp{3.f};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Data|PlayerInfo")
	int32 fh_PlayerCurWeaponSlot{0};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Data|PlayerInfo")
	FString fh_PlayerWeaponIDSet{"0,0"};
};
