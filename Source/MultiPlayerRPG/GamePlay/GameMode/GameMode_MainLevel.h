#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "GameMode_MainLevel.generated.h"

class APlayerController_FH;
class UFH_ConnectionObject;
class UGameInstanceRPG;

UCLASS()
class MULTIPLAYERRPG_API AGameMode_MainLevel : public AGameMode
{
	GENERATED_BODY()

public:
	AGameMode_MainLevel();

protected:
	virtual void BeginPlay() override;
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void Logout(AController* Exiting) override;
	virtual void Destroyed() override;

/* My Code */
	// Property
private:
	UPROPERTY()
	int Index{0};
	
	/*
	 * MySQL
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess=true), Category="GameMode|MySQlConnector")
	UFH_ConnectionObject* Connector;

public:
	/*
	 * GamePlay
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess=true), Category="GameMode|PlayerControllerMap")
	TMap<int, APlayerController_FH*> Fh_MapPlayerController;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess=true), Category="GameMode|GameInstance")
	UGameInstanceRPG* Fh_GameInstance;

public:
	UFUNCTION()
	void FH_UpdateUserLoginStatusByPC(APlayerController_FH* PC, const bool IsLogin);
	UFUNCTION()
	void FH_Logout(APlayerController_FH* PlayerController_Fh);
};
