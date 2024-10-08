#pragma once

#include "VisualLogger/VisualLoggerDebugSnapshotInterface.h"
#include "UtilityManager.generated.h"

class UAction;
class UScorerObject;
class UBlackboardComponent;

UCLASS(ClassGroup = (AI), meta = (BlueprintSpawnableComponent))
class DYNAMICAI_SYSTEM_API UUtilityManager : public UActorComponent, public IVisualLoggerDebugSnapshotInterface
{
    GENERATED_BODY()

    TObjectPtr<UAction> CurrentAction;
    FTimerHandle evaluationTimer;

    void SetEvaluationIterval(float NewInterval) { EvaluationInterval = NewInterval; }

    TMap<TSubclassOf<UScorerObject>, TObjectPtr<UScorerObject>> Scorers;

public:
    UUtilityManager() : Super() { PrimaryComponentTick.bCanEverTick = false; }

    void BeginPlay() override;

    /** @return related actions array */
    virtual TArray<UAction *> GetActions() { return Actions; };

protected:
    UPROPERTY(EditDefaultsOnly, Category = "UtilityManager|Blackboard")
    TObjectPtr<UBlackboardComponent> Blackboard;

    /** Set of all possible actions for character*/
    UPROPERTY(EditAnywhere, Instanced, Category = "UtilityManager|Actions")
    TArray<TObjectPtr<UAction>> Actions;

    UPROPERTY(EditAnywhere, Category = "UtilityManager|Evaluator")
    float EvaluationInterval = .1f;

public:
    /** @return currently executing action */
    UFUNCTION(BlueprintCallable, Category = "UtilityManager", DisplayName = "GetCurrentAction")
    UAction *BP_GetCurrentAction() const { return GetCurrentAction(); }
    virtual UAction *GetCurrentAction() const { return CurrentAction; }

    UFUNCTION(BlueprintCallable, Category = "UtilityManager")
    void SetScorerValue(TSubclassOf<UScorerObject> InScorer, float InValue);

private:
    virtual void EvaluateActions();
};