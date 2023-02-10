#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MultiPlayerRPG/Data/FFhUserInfoData.h"
#include "MultiPlayerRPG/Data/FFhPlayerInfoData.h"
#include "MultiPlayerRPG/Data/FFhWeaponInfoData.h"
#include "MultiPlayerRPG/GlobalVariable/FHColorEnum.h"
#include "Character_FH.generated.h"

class APlayerController_FH;
class USpringArmComponent;
class UCameraComponent;
struct FFhUserInfoData;
struct FFhPlayerInfoData;
struct FFhWeaponInfoData;

UCLASS()
class MULTIPLAYERRPG_API ACharacter_FH : public ACharacter
{
	GENERATED_BODY()

public:
	ACharacter_FH();

protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void Tick(float DeltaSeconds) override;

/* My Code */
	// Property
private:
	/*
	 * UserInfo PlayerInfo WeaponInfo
	 */
	UPROPERTY(Replicated, BlueprintReadWrite, Category="FH|PlayerController|UserData", meta=(AllowPrivateAccess=true))
	FFhUserInfoData Fh_UserInfoData;
	UPROPERTY(Replicated, BlueprintReadWrite, Category="FH|PlayerController|PlayerData", meta=(AllowPrivateAccess=true))
	FFhPlayerInfoData Fh_PlayerInfoData;
	UPROPERTY(Replicated, BlueprintReadWrite, Category="FH|PlayerController|SwordData", meta=(AllowPrivateAccess=true))
	FFhWeaponInfoData Fh_WeaponInfoFirstSlotData;
	UPROPERTY(Replicated, BlueprintReadWrite, Category="FH|PlayerController|GunData", meta=(AllowPrivateAccess=true))
	FFhWeaponInfoData Fh_WeaponInfoSecondSlotData;

	/*
	 * Component
	 */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category="FH|Player|Component", meta=(AllowPrivateAccess=true))
	USpringArmComponent* Fh_PlayerSpringArmComponent;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category="FH|Player|Component", meta=(AllowPrivateAccess=true))
	UCameraComponent* Fh_PlayerCameraComponent;

	/*
	 * GamePlay
	 */
	UPROPERTY()
	APlayerController_FH* Fh_OwnerController;
	
	// Function
public:
	/*
	 * Controller Delegate Input Bind
	 */
	UFUNCTION() void FH_OnMove(const EAxis::Type Axis, const float Value);
	UFUNCTION() void FH_OnMoveForward(const float Value);
	UFUNCTION() void FH_OnMoveRight(const float Value);
	UFUNCTION() void FH_OnPitchCamera(const float Value);
	UFUNCTION() void FH_OnYawCamera(const float Value);
	UFUNCTION() void FH_OnJump();
	UFUNCTION() void FH_OnStopJumping();

	/*
	 * Init UseInfo, PlayerInfo, WeaponInfo From Controller
	 */
	UFUNCTION(Server, Reliable)
	void FH_ServerInitCharacterInfoData();

	UFUNCTION(BlueprintImplementableEvent, Category="FH|Player|Color")
	void FH_UpdateCharacterColor(EFH_Color Color);
};
