#include "Core/UtilityManager.h"
#include "Utility/Service.h"
#include "Utility/Action.h"
#include "AIController.h"

DEFINE_LOG_CATEGORY(UtilityManagerLog);

UUtilityManager::UUtilityManager()
{
    PrimaryComponentTick.bCanEverTick = true;
    bAutoActivate = true;
}

void UUtilityManager::BeginPlay()
{
    if (IsEmpty())
    {
        UE_LOG(UtilityManagerLog, Warning, TEXT("No one action is defined"));
        return Super::BeginPlay();
    }

    for (UService *_service : Services)
    {
        if (_service)
        {
            _service->Init(this);
            _service->Activate();
        }
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
    AbortActiveActions();

    for (UService *_service : Services)
        if (_service)
            _service->Deactivate();

    Super::Deactivate();
    UE_LOG(UtilityManagerLog, Log, TEXT("[%s]: UtilityManager deactivated"), *GetOwner()->GetName());
}

void UUtilityManager::InitPools()
{
    _pools = ActionsPools.Array();
    /*Store all actions from pools to global pool*/
    for (FActionsPool &_pool : _pools)
    {
        _pool.Init(this);
        for (TObjectPtr<UAction> &_action : _pool.GetActions())
            if (!_actions.Contains(_action))
                _actions.Add(_action);
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
    for (UAction *_action : _activeActions)
    {
        if (_action->IsFinished)
            _activeActions.Remove(_action);
    }

    _pools.Sort([](const FActionsPool &p1, const FActionsPool &p2)
                { return p1.Priority > p2.Priority; });

    for (FActionsPool &_pool : _pools)
    {
        UAction *_evaluatedAction = _pool.EvaluateActions();
        if (_evaluatedAction &&
            CanRunConcurent(_evaluatedAction))
        {
            _activeActions.Add(_evaluatedAction);
            _evaluatedAction->Execute();
            return;
        }
    }
}

void UUtilityManager::AbortActiveActions()
{
    for (UAction *_action : _activeActions)
    {
        _action->FinishExecute(EExecutionResult::Aborted);
        UE_LOG(UtilityManagerLog, Log, TEXT("%s: action aborted"), *_action->GetName());
    }
}

void UUtilityManager::AbortAction(TSubclassOf<UAction> action)
{
    for (UAction *_action : _activeActions)
    {
        if (_action->GetClass() == action)
        {
            _action->FinishExecute(EExecutionResult::Aborted);
            return;
        }
    }
}

void UUtilityManager::AbortActionsFromPool(FName PoolName)
{
    FActionsPool *_pool = _pools.FindByPredicate([PoolName](const FActionsPool &p)
                                                 { return p.PoolName == PoolName; });
    if (_pool)
    {
        for (UAction *_action : _pool->GetActions())
            if (_action && _activeActions.Contains(_action))
                _action->FinishExecute(EExecutionResult::Aborted);
    }
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

bool UUtilityManager::SetPoolPriority(FName PoolName, int32 Priority)
{
    FActionsPool *_pool = _pools.FindByPredicate([PoolName](const FActionsPool &elem)
                                                 { return elem.PoolName == PoolName; });
    if (_pool)
    {
        _pool->SetPoolPriority(Priority);
        return true;
    }
    UE_LOG(UtilityManagerLog, Warning, TEXT("Pool with name [%s] not found"), *PoolName.ToString());
    return false;
}

#if WITH_EDITOR
void UUtilityManager::PostEditChangeProperty(FPropertyChangedEvent &PropertyChangedEvent)
{
    FName property = PropertyChangedEvent.GetPropertyName();
    if (((property == GET_MEMBER_NAME_CHECKED(FActionsPool, Actions) && ScorersCurveTable != nullptr) ||
         property == GET_MEMBER_NAME_CHECKED(UUtilityManager, ScorersCurveTable)) &&
        PropertyChangedEvent.ChangeType == EPropertyChangeType::ValueSet)
        UpdateScorersCurveTable();

    if (((property == GET_MEMBER_NAME_CHECKED(FActionsPool, Actions) && ActionModifiersCurveTable != nullptr) ||
         property == GET_MEMBER_NAME_CHECKED(UUtilityManager, ActionModifiersCurveTable)) &&
        PropertyChangedEvent.ChangeType == EPropertyChangeType::ValueSet)
        UpdateModifiersCurveTable();
    Modify();
}

void UUtilityManager::UpdateScorersCurveTable()
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
                    CreateCurve(tag.GetTagName(), *ScorersCurveTable); // if curve is not exist, create new curve in table
            }
        }
    }
}

void UUtilityManager::UpdateModifiersCurveTable()
{

    if (!IsValid(ActionModifiersCurveTable))
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
            FName _actionName = FName(action->GetName().LeftChop(4));
            if (!ActionModifiersCurveTable->FindRichCurve(_actionName, FString(), false))
            {
                CreateCurve(_actionName, *ActionModifiersCurveTable);
            }
        }
    }
}

void UUtilityManager::CreateCurve(FName CurveName, UCurveTable &InTable)
{
    FRichCurve &newCurve = InTable.AddRichCurve(CurveName);
    /*Set default keys for new curve*/
    newCurve.UpdateOrAddKey(0, 0);
    newCurve.UpdateOrAddKey(1, 1);
    InTable.Modify();
}

void UUtilityManager::ResetConsiderations()
{
    _considerations.Empty();
    UE_LOG(UtilityManagerLog, Log, TEXT("Considerations map is cleared"));
}

#endif // WITH_EDITOR

void FActionsPool::Init(UObject* outer)
{
    for (TSubclassOf<UAction> &_actionClass : Actions)
    {
        UAction *_action = NewObject<UAction>(outer, _actionClass);
        if (_action)
            _actions.Add(_action);
    }
}

TObjectPtr<UAction> FActionsPool::EvaluateActions()
{
    UAction *_bestAction = nullptr;
    float maxScore = 0;

    // Actions.Sort([](const UAction &_a1, const UAction &_a2)
    //              { return _a1.ActionPriority > _a2.ActionPriority; });

    for (UAction *action : _actions)
    {
        if (!action || !action->CanBeEvaluated())
            continue;
        float currentScore = action->EvaluateActionScore();
        if ((currentScore > maxScore) && (currentScore > ScoreThresshold))
        {
            maxScore = currentScore;
            _bestAction = action;
        }
    }
    return _bestAction;
}