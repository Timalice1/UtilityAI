#pragma once
#include "CoreMinimal.h"
#include "UtilityInterface.generated.h"

UINTERFACE()
class DYNAMICAI_SYSTEM_API UUtilityInterface : public UInterface
{
    GENERATED_BODY()
};

class DYNAMICAI_SYSTEM_API IUtilityInterface
{
    GENERATED_BODY()
public:
    /*Override this event and update all neceessary scorers values directly in controller/pawn*/
    UFUNCTION(BlueprintImplementableEvent, Category = "UtilityManagerInterface")
    void UpdateScorers();
};