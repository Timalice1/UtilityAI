#pragma once

#include "CoreMinimal.h"
#include "GameplayTags.h"
#include "Scorer.generated.h"

UCLASS(MinimalAPI, BlueprintType)
class UConsideration : public UObject
{
    GENERATED_BODY()

private:
    float _value;
    FRichCurve _responseCurve;

public:
    UConsideration() {};

    virtual void SetResponseCurve(FRichCurve inCurve)
    {
        _responseCurve = inCurve;
    }

    virtual void SetValue(float NewValue)
    {
        _value = NewValue;
    }

    virtual float GetRawScore()
    {
        if (!_responseCurve.IsEmpty())
            return _responseCurve.Eval(_value);
        return 0.f;
    }
};

UENUM(BlueprintType)
enum EOperator : uint8
{
    AND,
    OR,
};

USTRUCT(BlueprintType)
struct FScorer
{
    GENERATED_BODY()
    
protected:
    UPROPERTY()
    TObjectPtr<UConsideration> _considerationInstance;

public:
    UPROPERTY(VisibleInstanceOnly)
    bool FirstElement = false;

    UPROPERTY(EditDefaultsOnly, meta = (EditCondition = "!FirstElement", EditConditionHides))
    TEnumAsByte<EOperator> Operator;

    /** Returns (1-x) value */
    UPROPERTY(EditDefaultsOnly)
    bool Inverted = false;

    /** Scorer implementation class */
    UPROPERTY(EditDefaultsOnly)
    FGameplayTag ScorerTag;

    /** Weight of this consideration for current action */
    UPROPERTY(EditDefaultsOnly, meta = (ClampMin = 0.1))
    float Weight = 1.f;

    FORCEINLINE virtual void SetConsiderationInstance(TObjectPtr<UConsideration> inInstance)
    {
        _considerationInstance = inInstance;
        UE_CLOG(_considerationInstance != nullptr, LogTemp, Log, TEXT("Instance [%s] created"), *_considerationInstance->GetName());
    }

    FORCEINLINE virtual TObjectPtr<UConsideration> GetConsiderationInstance() { return _considerationInstance; }

    FORCEINLINE virtual float GetRawScore() {
        if (IsValid(_considerationInstance))
            return _considerationInstance->GetRawScore();
        return 0.f;
    }

    virtual float GetScore()
    {
        if (_considerationInstance != nullptr)
        {
            float rawScore = _considerationInstance->GetRawScore();
            rawScore = Inverted ? 1 - rawScore : rawScore;
            rawScore *= Weight;
            return rawScore;
        }
        return 0.f;
    }

    friend bool operator==(const FScorer &This, const FScorer &Other)
    {
        return This.ScorerTag == Other.ScorerTag;
    }

    friend uint32 GetTypeHash(const FScorer &scorer) { return GetTypeHash(scorer.ScorerTag); }
};

USTRUCT(BlueprintType)
struct FScorerPool
{
    GENERATED_BODY()

    UPROPERTY(VisibleInstanceOnly)
    bool FirstElement = false;

    UPROPERTY(EditDefaultsOnly, meta = (EditCondition = "!FirstElement", EditConditionHides))
    TEnumAsByte<EOperator> Operator;

    UPROPERTY(EditDefaultsOnly)
    TArray<FScorer> Scorers;

    float GetScore() 
    {
        float _totalScore = Scorers[0].GetScore();
        for (int i = 1; i < Scorers.Num(); i++)
        {
            float currentScore = Scorers[i].GetScore();

            if (Scorers[i].Operator == OR)
                _totalScore = FMath::Clamp(_totalScore + currentScore, 0, 1);
            else
                _totalScore *= currentScore;
        }
        return _totalScore;
    }

    TArray<FScorer> GetScorers()
    {
        return Scorers;
    }
};