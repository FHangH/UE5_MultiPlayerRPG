#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI_Start.generated.h"

UCLASS()
class MULTIPLAYERRPG_API UUI_Start : public UUserWidget
{
	GENERATED_BODY()

/* My Code */
	// Property
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UI|Start")
	FString fh_ServerIP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UI|Start")
	FString fh_UserName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UI|Start")
	FString fh_UserEmail;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UI|Start")
	FString fh_UserPassWord;
	
};