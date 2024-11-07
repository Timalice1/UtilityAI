#pragma once

#include "Utility/Action.h"
#include "FallbackAction.generated.h"

UCLASS(Blueprintable, BlueprintType, MinimalAPI, HideCategories = ActionConfig)
class UFallbackAction : public UAction
{
    GENERATED_BODY()

public:
    UFallbackAction() : Super()
    {
        ScoreType = EST_Constant;
    };
};