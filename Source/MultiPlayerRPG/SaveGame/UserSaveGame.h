#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "UserSaveGame.generated.h"

UCLASS()
class MULTIPLAYERRPG_API UUserSaveGame : public USaveGame
{
	GENERATED_BODY()
	
/* My Code */	
	// Property
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess=true), Category="SaveGame|LoginInfo")
	FString fh_UserID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess=true), Category="SaveGame|LoginInfo")
	FString fh_UserName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess=true), Category="SaveGame|LoginInfo")
	FString fh_UserEmail;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess=true), Category="SaveGame|LoginInfo")
	FString fh_UserPassWord;
};