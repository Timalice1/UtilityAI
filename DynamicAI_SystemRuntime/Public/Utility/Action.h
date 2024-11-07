#pragma once

#include "GameplayTags.h"
#include "CoreMinimal.h"
#include "Action.generated.h"

UENUM(BlueprintType)
enum EScoreType : uint8
{
    EST_Constant UMETA(DisplayName = "Constant"),
    EST_Evaluated UMETA(DisplayName = "Evaluated")
    // EST_Blackboard
};

class AAIController;
class APawn;
class UService;
class UUtilityManager;
class UConsideration;
struct FScorer;

UCLASS(MinimalAPI,
       BlueprintType,
       Blueprintable,
       Abstract,
       DefaultToInstanced,
       EditInlineNew,
       DisplayName = "UtilityAction")
class UAction : public UObject
{
    GENERATED_BODY()


    float ActionScore = 0.f;

private:
    virtual UWorld *GetWorld() const override;

#if WITH_EDITOR
    virtual void PostEditChangeProperty(FPropertyChangedEvent &PropertyChangedEvent) override;
#endif

public:
    UPROPERTY(BlueprintReadOnly, Category = "UtilityAI|Action")
    TObjectPtr<AAIController> _controller;
    UPROPERTY(BlueprintReadOnly, Category = "UtilityAI|Action")
    TObjectPtr<APawn> _pawn;
    UPROPERTY(BlueprintReadOnly, Category = "UtilityAI|Action")
    TObjectPtr<UUtilityManager> _manager;

    /** Set this to true if action can be run in parallel with others*/
    UPROPERTY(EditDefaultsOnly, Category = "ActionConfig")
    bool bCanRunConcurent = false;

    /** Priority of this action within pool */
    UPROPERTY(EditAnywhere, Category = "ActionConfig", meta = (ClampMin = 0.01f))
    float ActionPriority = 1;

    UPROPERTY(EditDefaultsOnly, Category = "ActionConfig")
    TEnumAsByte<EScoreType> ScoreType;

    UPROPERTY(EditDefaultsOnly, Category = "ActionConfig",
              meta = (ClampMin = 0.f, ClampMax = 1.f,
                      EditCondition = "ScoreType==EScoreType::EST_Constant",
                      EditConditionHides, EditInline = false))
    float ConstantScore = 0.f;

    UPROPERTY(EditDefaultsOnly, Category = "ActionConfig",
              meta = (EditCondition = "ScoreType==EScoreType::EST_Evaluated",
                      EditConditionHides,
                      EditInline = false))
    TArray<FScorer> Scorers;

    UFUNCTION(BlueprintCallable, Category = "UtilityAI|Action", BlueprintPure)
    UUtilityManager *GetUtilityManager() { return _manager; }

    /** Provides action implementation */
    UFUNCTION(BlueprintImplementableEvent, Category = "UtilityAI|Action")
    void ExecuteAction(AAIController *Controller, APawn *Pawn);

    /** Called when action is finished or aborted */
    UFUNCTION(BlueprintImplementableEvent, Category = "UtilityAI|Action")
    void OnActionFinished(AAIController *Controller, APawn *Pawn);

    /** Needs to be called when action is about to finish */
    UFUNCTION(BlueprintCallable, Category = "UtilityAI|Action")
    void FinishExecute();

public:
    bool IsFinished = true;

    UAction(){};

    /** Initialize action */
    virtual void Init(TObjectPtr<UUtilityManager> InManager, TMap<FGameplayTag, TObjectPtr<UConsideration>> InConsiderations);

    /** Calculate action score based on score type and linked scorers */
    virtual float EvaluateActionScore();

    /** Call's action implementation */
    virtual void Execute();

    virtual float GetActionScore() const { return ActionScore; }

    virtual bool CanRunConcurrentlyWith(UAction* OtherAction) const
    {
        return bCanRunConcurent && OtherAction->bCanRunConcurent;
    }

    /** @return all scorers tags related to this action*/
    virtual TArray<FScorer> GetScorers() { return Scorers; };
};