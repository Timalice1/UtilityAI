#include "Utility/Action.h"

UWorld *UAction::GetWorld() const
{
    if (GIsEditor && !GIsPlayInEditorWorld)
        return nullptr;
    else if (GetOuter())
        return GetOuter()->GetWorld();
    return nullptr;
}

void UAction::Init(AAIController *InController, APawn *InPawn)
{
    this->controller = InController;
    this->pawn = InPawn;

    for (FScorer &scorer : Scorers)
        scorer.SetScorerInstance(NewObject<UScorerObject>(this, scorer.ScorerClass));
}

float UAction::EvaluateActionScore()
{
    if (ScoreType == EST_Constant)
        ActionScore = ConstantScore;

    if (!Scorers.IsEmpty())
    {
        ActionScore = Scorers[0].GetScore();
        for (int i = 1; i < Scorers.Num(); i++)
        {
            float currentScore = Scorers[i].GetScore();
            if (Scorers[i].Operator == OR)
                ActionScore += currentScore;
            else
                ActionScore *= currentScore;
        }
    }

    return ActionScore;
}

TArray<TObjectPtr<UScorerObject>> UAction::GetScorers()
{
    TArray<TObjectPtr<UScorerObject>> instances;

    for (FScorer &scorer : Scorers)
        instances.Add(scorer.GetScorerInstance());

    return instances;
}
