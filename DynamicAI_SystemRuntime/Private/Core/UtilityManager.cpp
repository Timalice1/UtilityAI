#include "Core/UtilityManager.h"
#include "Utility/Service.h"
#include "Utility/Action.h"
#include "Utility/FallbackAction.h"
#include "AIController.h"

DEFINE_LOG_CATEGORY(UtilityManagerLog);

UUtilityManager::UUtilityManager()
{
    PrimaryComponentTick.bCanEverTick = true;
    bAutoActivate = true;
}

void UUtilityManager::BeginPlay()
{
    for (UService *_service : Services)
        _service->Init(this);

    if (IsEmpty())
    {
        UE_LOG(UtilityManagerLog, Warning, TEXT("No one action is defined"));
        return Super::BeginPlay();
    }

    for (auto &_action : _actionsScorers)
        _action.Key->Init(this, _action.Value);
    _actionsScorers.Empty();

    Super::BeginPlay();
}

void UUtilityManager::PostReinitProperties()
{
    if (GIsEditor && !GIsPlayInEditorWorld)
        return Super::PostReinitProperties();

    if (IsEmpty())
    {
        UE_LOG(UtilityManagerLog, Warning, TEXT("No one action is defined"));
        return Super::PostReinitProperties();
    }

    /*Init properies before owner begin play */
    InitPools();
    InitScorers();

    Super::PostReinitProperties();
}

void UUtilityManager::Activate(bool bReset)
{
    if (GIsEditor && !GIsPlayInEditorWorld)
        return Super::Activate(bReset);

    GetWorld()
        ->GetTimerManager()
        .SetTimer(evaluationTimer, this, &ThisClass::EvaluateActions, EvaluationInterval, true);

    Super::Activate(bReset);
    UE_LOG(UtilityManagerLog, Log, TEXT("[%s]: UtilityManager activated"), *GetOwner()->GetName());
}

void UUtilityManager::Deactivate()
{
    GetWorld()->GetTimerManager().ClearAllTimersForObject(this);

    for (UService *_service : Services)
    {
        GetWorld()->GetTimerManager().ClearTimer(_service->tickTimer);
        _service = nullptr;
        _service->ConditionalBeginDestroy();
    }

    Services.Empty();

    _actions.Empty();
    _pools.Empty();
    _activeActions.Empty();
    _considerations.Empty();

    Super::Deactivate();
    UE_LOG(UtilityManagerLog, Log, TEXT("[%s]: UtilityManager deactivated"), *GetOwner()->GetName());
}

void UUtilityManager::InitPools()
{
    _pools = ActionsPools.Array();
    /*Store all actions from pools to global pool*/
    for (FActionsPool &_pool : _pools)
    {
        for (TObjectPtr<UAction> &_action : _pool.GetActions())
        {
            if (_action && !_actions.Contains(_action))
                _actions.Add(_action);
        }
    }
}

bool UUtilityManager::InitScorers()
{
    /*Initialize scorers consideration references*/
    for (TObjectPtr<UAction> &action : _actions)
    {
        if (!action)
            continue;
        TMap<FGameplayTag, TObjectPtr<UConsideration>> _actionConsiderations;
        for (const FScorer &scorer : action->GetScorers())
        {
            auto tag = scorer.ScorerTag;
            if (!tag.IsValid())
                continue;
            if (!_considerations.Contains(tag))
                _actionConsiderations.Add(tag, CreateScorer(tag));
            else
                _actionConsiderations.Add(tag, *_considerations.Find(tag));
        }
        _actionsScorers.Add(action, _actionConsiderations);
    }
    return true;
}

UConsideration *UUtilityManager::CreateScorer(FGameplayTag scorerTag)
{
    UConsideration *_newCons = NewObject<UConsideration>(this, UConsideration::StaticClass(), scorerTag.GetTagName());
    _considerations.Add(scorerTag, _newCons);
    return _newCons;
}

bool UUtilityManager::IsEmpty()
{
    for (auto &pool : ActionsPools)
    {
        if (!pool.IsEmpty())
            return false;
    }
    return true;
}

void UUtilityManager::EvaluateActions()
{
    _activeActions.RemoveAll([](UAction *_action)
                             { return _action->IsFinished(); });

    _pools.Sort([](const FActionsPool &p1, const FActionsPool &p2)
                { return p1.Priority > p2.Priority; });

    for (FActionsPool &_pool : _pools)
    {
        UAction *_evaluatedAction = _pool.EvaluateActions();
        if (_evaluatedAction && CanRunConcurent(_evaluatedAction))
        {
            _activeActions.Add(_evaluatedAction);
            _evaluatedAction->Execute();
            FallbackAction->FinishExecute();
            return;
        }
        FallbackAction->Execute();
    }
}

void UUtilityManager::AbortActiveActions()
{
    for (UAction *_action : _activeActions)
        _action->FinishExecute();
}

bool UUtilityManager::CanRunConcurent(UAction *Action) const
{
    if (_activeActions.IsEmpty())
        return true;

    for (UAction *_action : _activeActions)
    {
        if (Action == _action || !Action->CanRunConcurrentlyWith(_action))
        {
            return false;
        }
    }
    return true;
}

void UUtilityManager::SetScorerValue(FGameplayTag InScorerTag, float InValue)
{   
    if (AController *controller = Cast<AController>(GetOwner()))
    {
        if (!controller->GetPawn())
            return;
    }

    if (_considerations.IsEmpty())
    {
        UE_LOG(UtilityManagerLog, Warning, TEXT("UtilityManager::SetScorerValue(%s) - no one scorer is defined."),
               *InScorerTag.GetTagName().ToString());
        return;
    }

    if (!InScorerTag.IsValid())
    {
        UE_LOG(UtilityManagerLog, Warning,
               TEXT("UtilityManager::SetScorerValue - tag is invalid"));
        return;
    }

    if (!_considerations.Contains(InScorerTag))
        return;

    TObjectPtr<UConsideration> _cons = *_considerations.Find(InScorerTag);
    _cons->SetValue(InValue);
}

void UUtilityManager::ResetScorers(FGameplayTagContainer InScorersTags)
{
    if (InScorersTags.IsEmpty())
    {
        UE_LOG(UtilityManagerLog, Warning, TEXT("UUtilityManager::ResetScorers() - input parameter [InScorersTags] is empty"));
        return;
    }

    TArray<FGameplayTag> _tags;
    InScorersTags.GetGameplayTagArray(_tags);
    for (const FGameplayTag &_tag : _tags)
        SetScorerValue(_tag, 0.f);
}

#if WITH_EDITOR
void UUtilityManager::PostEditChangeProperty(FPropertyChangedEvent &PropertyChangedEvent)
{
    FName property = PropertyChangedEvent.GetPropertyName();
    if ((property == GET_MEMBER_NAME_CHECKED(FScorer, ScorerTag) ||
         property == GET_MEMBER_NAME_CHECKED(FActionsPool, Actions) ||
         property == GET_MEMBER_NAME_CHECKED(UUtilityManager, ScorersCurveTable)) &&
        PropertyChangedEvent.ChangeType == EPropertyChangeType::ValueSet)
        UpdateCurveTableRows();
    Modify();
}

void UUtilityManager::UpdateCurveTableRows()
{
    if (IsEmpty())
    {
        ResetConsiderations();
        return;
    }

    if (!IsValid(ScorersCurveTable))
    {
        UE_LOG(UtilityManagerLog, Warning, TEXT("Property [ScorersCurveTable] is invalid!"));
        return;
    }

    for (FActionsPool &pool : ActionsPools)
    {
        /*Get scorers array from each referenced action*/
        for (const TObjectPtr<UAction> &action : pool.GetActions())
        {
            if (!action)
                continue;

            for (FScorer &scorer : action->GetScorers())
            {
                /*Get a scorer tag from each scorer in action*/
                FGameplayTag tag = scorer.ScorerTag;
                if (tag.IsValid() && !ScorersCurveTable->FindRichCurve(tag.GetTagName(), FString(), false))
                    CreateCurve(tag.GetTagName()); // if curve is not exist, create new curve in table
            }
        }
    }
}

void UUtilityManager::CreateCurve(FName Name)
{
    FRichCurve &newCurve = ScorersCurveTable->AddRichCurve(Name);
    /*Set default keys for new curve*/
    newCurve.UpdateOrAddKey(0, 0);
    newCurve.UpdateOrAddKey(1, 1);
    ScorersCurveTable->Modify();
}

void UUtilityManager::ResetConsiderations()
{
    _considerations.Empty();
    UE_LOG(UtilityManagerLog, Log, TEXT("Considerations map is cleared"));
}

#endif // WITH_EDITOR

TObjectPtr<UAction> FActionsPool::EvaluateActions()
{
    UAction *_bestAction = nullptr;
    float maxScore = 0;
    for (UAction *action : Actions)
    {
        if (!action || !action->CanBeEvaluated())
            continue;
        float currentScore = action->EvaluateActionScore();
        if (currentScore > maxScore && currentScore > ScoreThresshold)
        {
            maxScore = currentScore;
            _bestAction = action;
        }
    }
    return _bestAction;
}