#include "Utility/UtilityManager.h"
#include "Utility/Action.h"
#include "AIController.h"

void UUtilityManager::BeginPlay()
{

    if (Actions.IsEmpty())
    {
        UE_LOG(LogTemp, Warning, TEXT("[%s]: no one actions in defined"), *GetName());
        return;
    }

    // Initialize available actions
    for (const TObjectPtr<UAction> &action : Actions)
    {
        if (AAIController *controller = Cast<AAIController>(GetOwner()))
            action->Init(controller, controller->GetPawn());
        if (APawn *pawn = Cast<APawn>(GetOwner()))
            action->Init(Cast<AAIController>(pawn->GetController()), pawn);

        for (const TObjectPtr<UScorerObject> &scorer : action->GetScorers())
        {
            if (!Scorers.Contains(scorer->GetClass()))
                Scorers.Add(scorer->GetClass(), scorer);
        }
    }

    // Run action evaluator
    GetWorld()->GetTimerManager().SetTimer(
        evaluationTimer, this, &ThisClass::EvaluateActions, EvaluationInterval, true, 0);
}

void UUtilityManager::EvaluateActions()
{
    // Go throught each instanced action object, look for best scored action
    float maxScore = -1.f;

    for (const TObjectPtr<UAction> &action : Actions)
    {
        float currentScore = action->EvaluateActionScore();
        if (currentScore > maxScore)
        {
            maxScore = currentScore;
            this->CurrentAction = action;
        }
    }

    if (CurrentAction)
        CurrentAction->Execute();
}


void UUtilityManager::SetScorerValue(TSubclassOf<UScorerObject> InScorer, float InValue)
{
    if (TObjectPtr<UScorerObject> scorer = *Scorers.Find(InScorer))
        scorer->SetValue(InValue);
}
