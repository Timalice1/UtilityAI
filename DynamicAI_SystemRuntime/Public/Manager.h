#pragma once

#include "Manager.generated.h"

UCLASS(MinimalAPI, BlueprintType)
class UManager : public UObject
{
    GENERATED_BODY()

protected:
    UPROPERTY(EditDefaultsOnly, Category = Evaluator)
    float EvaluationInterval = .1f;

    UPROPERTY(EditAnywhere, Category = Actions, Instanced)
    TArray<class UAction *> Actions;
};