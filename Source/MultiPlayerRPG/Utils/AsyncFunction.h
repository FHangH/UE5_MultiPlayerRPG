#pragma once

#include "CoreMinimal.h"
#include "AsyncFunction.generated.h"

DECLARE_DELEGATE(FHDGAsyncFunc);

UCLASS()
class MULTIPLAYERRPG_API UAsyncFunction : public UObject
{
	GENERATED_BODY()
	
public:
	template<class T>
	FORCEINLINE static void ExecuteAsyncFunction(T* ClassT, typename TMemFunPtrType<false, T, void()>::Type Func)
	{
		FHDGAsyncFunc AsyncFunc;
		AsyncFunc.BindUObject(ClassT, Func);
		
		if (IsInGameThread())
		{
			if(AsyncFunc.ExecuteIfBound()){}
		}
		else
		{
			AsyncTask(ENamedThreads::GameThread, [&]()
			{
				if(AsyncFunc.ExecuteIfBound()){}
			});
		}
		AsyncFunc.Unbind();
	}
};