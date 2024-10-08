#pragma once

#include "CoreMinimal.h"
#include "Scorer.generated.h"

UCLASS(MinimalAPI, BlueprintType, Abstract, Blueprintable,
       DisplayName = "UtilityScorer")
class UScorerObject : public UObject
{
    GENERATED_BODY()

protected:
    UPROPERTY(EditAnywhere)
    FRuntimeFloatCurve ResponseCurve;

    float ScorerValue = .0f;

public:
    UScorerObject() {};

    virtual void SetValue(float NewValue) { ScorerValue = NewValue; }

    virtual float GetScore()
    {
        float score = ResponseCurve.GetRichCurve()->Eval(ScorerValue);
        score = FMath::Clamp(score, 0, 1);
        return score;
    }
};

UENUM(BlueprintType)
enum EOperator : uint8
{
    AND,
    OR
};

USTRUCT(BlueprintType)
struct FScorer
{
    GENERATED_BODY()

private:
    TObjectPtr<UScorerObject> ScorerInstance;

public:
    UPROPERTY(VisibleInstanceOnly, Transient)
    bool FirstElement = false;

    UPROPERTY(EditDefaultsOnly, meta = (EditCondition = "!FirstElement", EditConditionHides))
    TEnumAsByte<EOperator> Operator;

    UPROPERTY(EditDefaultsOnly)
    bool Inverted;

    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<UScorerObject> ScorerClass;

    void SetScorerInstance(TObjectPtr<UScorerObject> NewScorer) { ScorerInstance = NewScorer; };

    TObjectPtr<UScorerObject> GetScorerInstance() { return ScorerInstance; }

    float GetScore()
    {
        if (ScorerInstance)
            return Inverted ? 1 - ScorerInstance->GetScore() : ScorerInstance->GetScore();
        return 0.f;
    }

    friend uint32 GetTypeHash(const FScorer &scorer) { return GetTypeHash(scorer.ScorerClass); }
};