#pragma once
#include "CoreMinimal.h"
#include "UtilityInterface.generated.h"

class UUtilityManager;

UINTERFACE()
class DYNAMICAI_SYSTEM_API UUtilityInterface : public UInterface
{
    GENERATED_BODY()
};

class DYNAMICAI_SYSTEM_API IUtilityInterface
{
    GENERATED_BODY()
public:
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "UtilityManagerInterface")
    UUtilityManager* GetUtilityManager(); 
};