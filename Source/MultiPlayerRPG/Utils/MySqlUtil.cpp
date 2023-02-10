#include "MySqlUtil.h"
#include "MultiPlayerRPG/GlobalVariable/FHWeaponTypeEnum.h"

/*
 * Insert
 */
FString MySqlUtil::InsertIntoUserInfoSqlQuery(const FString Name, const FString Email, const FString Password)
{
	// NULL, 'Name', 'Email', 'Password', 0
	return FString::Printf(TEXT("NULL, '%s', '%s', '%s', 0"), *Name, *Email, *Password);
}

FString MySqlUtil::InsertIntoPlayerInfoDefaultDataSqlQuery(const FString ID)
{
	// INSERT INTO PlayerInfo VALUES(1000, 1, 100.0, 100.0, 1.0, 0.1, 1.1, 10.0, 'X=0.0 Y=0.0 Z=0.0', 'WHITE', 0.0, 3.0, 0, '(0.0)');
	const auto Location = FVector(0.f).ToString();
	return FString::Printf
	(TEXT("%s, 1, 100.0, 100.0, 1.0, 0.1, 1.1, 10.0, '%s', 'WHITE', 0.0, 3.0, 0, '(0.0)'"),
		*ID, *Location);
}

FString MySqlUtil::InsertIntoWeaponInfoDefaultDataSqlQuery(const FString ID, const FString WeaponType)
{
	// INSERT INTO WeaponInfo VALUES(NULL, %s, 'C', 3.0, 10.0, 0.2, '%s');
	if (WeaponType.Equals(WEAPONTYPE_SWORD))
	{
		return FString::Printf(TEXT("NULL, %s, 'C', 3.0, 10.0, 0.2, 'SWORD'"), *ID);
	}
	if (WeaponType.Equals(WEAPONTYPE_GUN))
	{
		return FString::Printf(TEXT("NULL, %s, 'C', 1.0, 3.5, 0.1, 'GUN'"), *ID);
	}
	return FString::Printf(TEXT("NULL, %s, 'C', 3.0, 10.0, 0.2, 'SWORD'"), *ID);
}

FString MySqlUtil::InsertIntoPropsInfoDefaultDataSqlQuery(const FString ID)
{
	// INSERT INTO PropsInfo VALUES(ID, 0, 0, 0);
	return FString::Printf(TEXT("%s, 0, 0, 0"), *ID);
}

/*
 * Select
 */
FString MySqlUtil::SelectWeaponIDSetFromWeaponInfoByConditionSqlQuery(const FString ConditionKey, const FString ConditionValue)
{
	// SELECT WeaponID FROM WeaponInfo WHERE UserID=1000;
	return FString::Printf(TEXT("SELECT WeaponID FROM WeaponInfo WHERE %s=%s;"), *ConditionKey, *ConditionValue);
}

FString MySqlUtil::SelectAllFromTableByConditionSqlQuery(const FString TableName, const FString ConditionKey,
	const FString ConditionValue)
{
	// SELECT * FROM TableName WHERE UserID=1000;
	return "SELECT * FROM " + TableName + " WHERE " + ConditionKey + "='" + ConditionValue + "';";
}

FString MySqlUtil::SelectExistedUserByConditionSqlQuery(const FString Name, const FString Email)
{
	// SELECT UserID FROM UserInfo WHERE UserName='' OR UserEmail='';
	return FString::Printf(TEXT("SELECT UserID FROM UserInfo WHERE UserName='%s' OR UserEmail='%s';"), *Name, *Email);
}

FString MySqlUtil::SelectUserIsGamingByConditionSqlQuery(const FString Name, const FString Password)
{
	// SELECT UserIsLogin FROM UserInfo WHERE UserName='' AND UserPassWord='';
	return FString::Printf(TEXT("SELECT UserIsLogin FROM UserInfo WHERE UserName='%s' AND UserPassWord='%s';"), *Name, *Password);
}

/*
 * Update
 */
FString MySqlUtil::UpdateTableSetColumnValueByConditionSqlQuery(const FString TableName, const FString SetWhatAndValue,
	const FString Condition)
{
	// UPDATE PlayerInfo SET PlayerWeaponIDSet='(1,2)' WHERE UserID=1000;
	return FString::Printf(TEXT("UPDATE %s SET %s WHERE %s;"), *TableName, *SetWhatAndValue, *Condition);
}
