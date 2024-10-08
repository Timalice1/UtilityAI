#pragma once

#include "Scorer.h"
#include "Action.generated.h"

UENUM(BlueprintType)
enum EScoreType : uint8
{
    EST_Constant UMETA(DisplayName = "Constant"),
    EST_Evaluated UMETA(DisplayName = "Evaluated")
};

class AAIController;
class APawn;

UCLASS(MinimalAPI, BlueprintType, Abstract, EditInlineNew,
       DisplayName = "Utility Action")
class UAction : public UObject
{
    GENERATED_BODY()

    TObjectPtr<AAIController> controller;
    TObjectPtr<APawn> pawn;

    float ActionScore = 0.f;

protected:
    /**Define action score type.
     * Constant if action return constant score and evaluated if score needs to be calculated
     * using linked scorers*/
    UPROPERTY(EditDefaultsOnly, Category = "ActionConfig", meta = (EditInline = false))
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

    /** Provides action implementation */
    UFUNCTION(BlueprintImplementableEvent, Category = "UtilityAI|Action")
    void ExecuteAction(AAIController *Controller, APawn *Pawn);

    /** Called when action is finished or aborted */
    UFUNCTION(BlueprintImplementableEvent, Category = "UtilityAI|Action")
    void OnActionFinished(AAIController *Controller, APawn *Pawn);

    /** Needs to be called when action is about to finish */
    UFUNCTION(BlueprintCallable, Category = "UtilityAi|Action")
    void FinishExecute() { OnActionFinished(controller, pawn); }

public:
    UAction() {};

    void PostEditChangeProperty(FPropertyChangedEvent &PropertyChangedEvent) override {
        FName property = PropertyChangedEvent.GetPropertyName();
        if(property == GET_MEMBER_NAME_CHECKED(UAction, Scorers) && !Scorers.IsEmpty()) {
            Scorers[0].FirstElement = true;
            for(int i = 1; i<Scorers.Num(); i++)
                Scorers[i].FirstElement = false;
        }
        Modify();
    };

    virtual UWorld *GetWorld() const override;

    /** Initialize action */
    virtual void Init(AAIController *InController, APawn *InPawn);

    /** Calculate action score based on score type and linked scorers */
    virtual float EvaluateActionScore();

    /** Call's action implementation */
    virtual void Execute()
    {
        if (controller && pawn)
            this->ExecuteAction(controller, pawn);
    }

    virtual float GetActionScore() const { return ActionScore; }

    /** @return all linked scorers instances */
    virtual TArray<TObjectPtr<UScorerObject>> GetScorers();
};