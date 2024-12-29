#include "Utility/Action.h"
#include "Utility/Service.h"
#include "Core/UtilityManager.h"
#include "AIController.h"
#include "Engine/CurveTable.h"

#if WITH_EDITOR
void UAction::PostEditChangeProperty(FPropertyChangedEvent &PropertyChangedEvent)
{
    FName property = PropertyChangedEvent.GetPropertyName();
    if (property == GET_MEMBER_NAME_CHECKED(UAction, Scorers) && !Scorers.IsEmpty())
    {
        Scorers[0].FirstElement = true;
        for (int i = 1; i < Scorers.Num(); i++)
            Scorers[i].FirstElement = false;
    }
    Modify();
}
#endif

void UAction::Init(TObjectPtr<UUtilityManager> InManager, TMap<FGameplayTag, TObjectPtr<UConsideration>> InConsiderations)
{
    if (!InManager)
        return;

    _manager = InManager;
    /*Initialize controller and controlled pawn references*/
    if (AAIController *Controller = Cast<AAIController>(_manager->GetOwner()))
    {
        _controller = Controller;
        _pawn = Controller->GetPawn();
    }
    else if (APawn *Pawn = Cast<APawn>(_manager->GetOwner()))
    {
        _controller = Cast<AAIController>(Pawn->GetInstigatorController());
        _pawn = Pawn;
    }

    if (InConsiderations.IsEmpty())
    {
        UE_LOG(UtilityManagerLog, Warning, TEXT("Can't init action [%s]: no one consideration is defined"), *GetName());
        return;
    }

    /*Assing considerations to their scorers*/
    for (FScorer &Scorer : Scorers)
    {
        TObjectPtr<UConsideration> _cons = *InConsiderations.Find(Scorer.ScorerTag);
        if (!_cons || Scorer.GetConsiderationInstance() != nullptr)
            continue;

        if (auto CurveTable = _manager->GetCurveTableAsset())
        {
            FRichCurve *Curve = CurveTable->FindRichCurve(Scorer.ScorerTag.GetTagName(), FString(), false);
            if (Curve)
            {
                _cons->SetResponseCurve(*Curve);
                Scorer.SetConsiderationInstance(_cons);
            }
        }
    }

    if (auto _modifiersTable = _manager->GetActionModifiersCurveTable())
        ModifierCurve = _modifiersTable->FindRichCurve(FName(GetName().LeftChop(4)), FString(), false);

    UE_LOG(UtilityManagerLog, Log, TEXT("Action [%s] initialized!"), *GetName());

    Modify();
}



UWorld *UAction::GetWorld() const
{
    if (GIsEditor && !GIsPlayInEditorWorld)
        return nullptr;
    else if (GetOuter())
        return GetOuter()->GetWorld();
    return nullptr;
}

float UAction::EvaluateActionScore()
{
    if (ScoreType == EST_Constant)
        ActionScore = ConstantScore;

    if (ScoreType == EST_Evaluated && !Scorers.IsEmpty())
    {
        ActionScore = Scorers[0].GetScore();
        for (int i = 1; i < Scorers.Num(); i++)
        {
            float currentScore = Scorers[i].GetScore();

            if (Scorers[i].Operator == OR)
                ActionScore = (ActionScore + currentScore) - (ActionScore * currentScore);
            else if (Scorers[i].Operator == AND)
                ActionScore *= currentScore;
        }
    }
    if (ModifierCurve != nullptr)
        ActionScore = ModifierCurve->Eval(ActionScore);

    return ActionScore *= ActionPriority;
}

void UAction::Execute()
{
    IsFinished = false;
    if (_controller && _pawn && !IsUnreachable())
        this->ExecuteAction(_controller, _pawn);
}

void UAction::FinishExecute(EExecutionResult execResult)
{
    IsFinished = true;
    OnActionFinished(_controller, _pawn, execResult);

    bCanBeEvaluated = false;
    GetWorld()->GetTimerManager().SetTimer(_timeoutTimer, this, &ThisClass::ResetTimeout, Timeout);
}

void UAction::ResetTimeout()
{
    bCanBeEvaluated = true;
    GetWorld()->GetTimerManager().ClearTimer(_timeoutTimer);
}
