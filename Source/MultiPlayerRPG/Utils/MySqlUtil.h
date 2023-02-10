#pragma once
#include "CoreMinimal.h"

class MULTIPLAYERRPG_API MySqlUtil
{
/* My Code */
	// Function
public:
	/*
	 * Insert
	 */
	static FString InsertIntoUserInfoSqlQuery(const FString Name, const FString Email, const FString Password);

	static FString InsertIntoPlayerInfoDefaultDataSqlQuery(const FString ID);
	
	static FString InsertIntoWeaponInfoDefaultDataSqlQuery(const FString ID, const FString WeaponType);

	static FString InsertIntoPropsInfoDefaultDataSqlQuery(const FString ID);

	/*
	 * Select
	 */
	static FString SelectWeaponIDSetFromWeaponInfoByConditionSqlQuery(const FString ConditionKey, const FString ConditionValue);

	static FString SelectAllFromTableByConditionSqlQuery(
		const FString TableName, const FString ConditionKey, const FString ConditionValue);

	static FString SelectExistedUserByConditionSqlQuery(const FString Name, const FString Email);

	static FString SelectUserIsGamingByConditionSqlQuery(const FString Name, const FString Password);

	/*
	 * Update
	 */
	static FString UpdateTableSetColumnValueByConditionSqlQuery(
		const FString TableName, const FString SetWhatAndValue, const FString Condition);
};