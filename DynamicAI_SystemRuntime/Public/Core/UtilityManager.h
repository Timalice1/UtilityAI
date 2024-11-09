#pragma once

#include "Utility/Scorer.h"
#include "GameplayTags.h"
#include "UtilityManager.generated.h"

// ⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡿⠿⢿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿
// ⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡟⠀⣠⣄⠙⢿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿
// ⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠃⠀⣿⣿⣷⣄⠙⠻⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠿⠋⠉⠙⢿⣿⣿⣿
// ⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⢀⢸⣿⣿⣿⣿⣷⣦⡈⠻⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠿⠛⣁⣴⣾⡇⠀⢸⣿⣿⣿
// ⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠸⢸⣿⣿⣿⣿⣿⣿⣿⣦⡈⠹⢿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡿⠛⢁⣤⣾⣿⣿⣿⠀⠃⣿⣿⣿⣿
// ⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣧⢰⠈⣿⣿⣿⣿⣿⣿⣿⣿⣿⣦⡀⠙⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡿⠛⢁⣤⣾⣿⣿⣿⣿⣿⠇⢀⠀⣽⣿⣿⣿
// ⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠸⠀⣿⣿⣿⣿⣿⣉⣉⣉⣉⣉⣉⣀⣈⣉⣉⣛⠛⠛⠿⠿⢿⡿⠛⢁⣤⣾⣿⣿⣿⣿⣿⣿⣿⡟⠀⡜⢠⣿⣿⣿⣿
// ⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠀⢸⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣷⣶⠂⣠⣾⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠃⢈⠇⣸⣿⣿⣿⣿
// ⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠀⢸⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡿⠀⡜⢀⣾⣿⣿⣿⣿
// ⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⢀⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡇⠀⠃⣼⣿⣿⣿⣿⣿
// ⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠇⣸⣿⠿⠿⠿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡇⠀⢰⣿⣿⣿⣿⣿⣿
// ⣿⣿⣿⣿⣿⣿⣿⣿⠿⠛⠀⠀⢁⣤⣶⠶⠂⠀⠉⠛⢿⣿⣿⣿⣿⣿⣿⣿⣿⠟⠋⣉⣤⡤⠀⠀⠈⠉⠛⠿⣿⣿⣿⣿⣿⣿⡇⠀⢿⣿⣿⣿⣿⣿⣿⣿
// ⣿⣿⣿⣿⣿⡏⠁⣤⡶⠊⢠⠼⠛⠉⠀⠀⠀⠀⠀⠀⠈⢻⣿⣿⣿⣿⣿⠟⠁⣰⠞⠋⠁⠀⠀⠀⠀⠀⠀⠀⠈⢻⣿⣿⣿⣿⡷⢆⠸⣿⣿⣿⣿⣿⣿⣿⣿
// ⣿⣿⣿⣿⣿⣿⣦⠈⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠿⠿⠿⠿⠏⠀⠈⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠙⣉⣩⣍⣁⠀⠀⠙⣿⣿⣿⣿⣿⣿⣿⣿⣿
// ⣿⣿⣿⣿⠟⠋⢁⣴⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣠⣤⣄⣀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣠⣿⣿⣿⣿⣿⣷⣦⡀⠻⣿⣿⣿⣿⣿⣿⣿⣿
// ⣿⣿⠀⢤⣶⣾⣿⣿⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢰⣿⣿⣿⣿⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣿⣿⣿⣿⣿⣿⣿⣿⣿⣦⡈⠻⣿⣿⣿⣿⣿
// ⣿⣿⠁⠈⠙⠿⣿⣿⣷⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣰⣿⣿⣿⣿⣿⣧⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠹⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣄⠘⣿⣿⣿⣿
// ⣿⢿⣷⣄⡀⠀⠀⠈⣹⣿⣦⣄⣀⠀⠀⢀⡀⠀⣴⣿⣿⣿⠋⣸⣿⣿⣿⣷⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⡀⢻⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡆⢹⣿⣿
// ⣿⣄⠠⣤⣤⣤⣶⣾⣿⣿⣿⣿⣿⣿⣿⣿⣀⠀⠹⠿⠟⠃⡀⠘⣿⣿⣿⡿⠀⣀⡀⠀⠀⠀⠀⠀⠀⠀⣀⠤⣆⠑⠀⠻⣿⣿⣿⣿⣿⣿⣿⣿⣿⣷⠈⣿
// ⣿⣿⣷⣄⡙⠛⠿⠿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣷⣤⡖⣀⣄⠐⡄⠈⠉⠀⢀⣴⣿⣿⣿⣶⣶⣶⣶⣾⣿⠯⠒⠁⢀⣴⡄⠘⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿
// ⣿⣿⣿⡿⠏⠀⠄⢰⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠋⢀⣤⠲⣞⣾⣿⣿⣿⣿⣿⣿⣿⣿⣿⡟⣿
// ⣿⣿⣿⣤⠀⠀⣠⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠛⢁⣴⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠟⢀⣾
// ⣿⣿⣿⡯⠀⢀⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡿⠋⣠⣿⣿
// ⣿⣿⣿⠃⠀⣸⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠟⢁⣼⣿⣿⣿
// ⣿⣿⣿⠇⢠⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡿⠉⢠⣿⣿⣿⣿⣿
// ⣿⣿⠏⠀⢹⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠁⣤⣿⣿⣿⣿⣿⣿
// ⡟⠁⡀⢀⣾⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠇⢰⣿⣿⣿⣿⣿⣿⣿
// ⣿⠏⢠⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠉⠀⣿⣿⣿⣿⣿⣿⣿⣿
// ⠁⡀⠁⣸⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠀⣤⣿⣿⣿⣿⣿⣿⣿⣿
// ⡿⠁⣼⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡀⣿⣿⣿⣿⣿⣿⣿⣿⣿

class UAction;
class UFallbackAction;

USTRUCT(BlueprintType)
struct FActionsPool
{
    GENERATED_BODY()

public:
    UPROPERTY(EditDefaultsOnly)
    FName PoolName = TEXT("Default");

    /**
     * ActionsPools with hightest priority will be evaluated first
     * Only if none of action from ActionsPool will selected, evaluator goes to lower ActionsPool.
     * Otherwise other ActionsPools are ignored.
     */
    UPROPERTY(EditDefaultsOnly, meta = (ClampMin = 1))
    int32 Priority = 1;

    /**
     * Minimal actions utility score.
     * If actions in ActionsPool has lover utility than this value, ActionsPool are ignored
     * and evaluator goest to next ActionsPool
     */
    UPROPERTY(EditDefaultsOnly, Category = "UtilityManager|Evaluator", meta = (ClampMin = 0.f, ClampMax = 1.f))
    float ScoreThresshold = .01f;

    UPROPERTY(EditAnywhere, Instanced)
    TArray<TObjectPtr<UAction>> Actions;

public:
    bool IsEmpty()
    {
        return Actions.IsEmpty();
    }

    TObjectPtr<UAction> EvaluateActions();

    TArray<TObjectPtr<UAction>> GetActions() { return Actions; }
    TArray<TObjectPtr<UAction>> GetActions() const { return Actions; }

    friend uint32 GetTypeHash(const FActionsPool &pool)
    {
        return FCrc::MemCrc32(&pool, sizeof(FActionsPool));
    }
};

#if WITH_EDITOR
DECLARE_LOG_CATEGORY_EXTERN(UtilityManagerLog, Log, All);
#endif

UCLASS(ClassGroup = (AI), meta = (BlueprintSpawnableComponent))
class DYNAMICAI_SYSTEM_API UUtilityManager : public UActorComponent
{
    GENERATED_BODY()

    FTimerHandle evaluationTimer;
    FTimerHandle executeActionTimer;

    TMap<FGameplayTag, TObjectPtr<UConsideration>> _considerations;
    TMap<UAction *,
         TMap<FGameplayTag, TObjectPtr<UConsideration>>>
        _actionsScorers;

    TArray<FActionsPool> _pools;
    TArray<TObjectPtr<UAction>> _actions;

    TArray<UAction *> _activeActions;

    bool bCanSetScorer = true;

private:
    virtual void PostReinitProperties() override;

    /** @return false if there is no pools provided or pools is empty */
    virtual bool IsEmpty();

    virtual void Activate(bool bReset = false) override;
    virtual void Deactivate() override;

    /** Initialize all privided actions and their scorers */
    virtual void InitPools();
    virtual bool InitScorers();
    virtual UConsideration *CreateScorer(FGameplayTag scorerTag) final;

    /**
     * Scoring each action from each pool, and execute action with best utility score.
     * Add selected action from the pool to active actions array is action can be executing
     * concurently with other actions
     */
    virtual void EvaluateActions();

    /** Check if given action can run concurently with all of active actions */
    virtual bool CanRunConcurent(UAction *Action) const;

private:
#if WITH_EDITOR
    virtual void PostEditChangeProperty(FPropertyChangedEvent &PropertyChangedEvent) override;
    virtual void UpdateCurveTableRows();
    virtual void CreateCurve(FName Name);
    virtual void ResetConsiderations();
#endif // WITH_EDITOR

public:
    UUtilityManager();

    void BeginPlay() override;

    virtual TArray<FActionsPool> GetPools() { return _pools; }
    virtual TArray<FActionsPool> GetPools() const { return _pools; }

    /** @return an array of all posiible actions */
    virtual TArray<TObjectPtr<UAction>> GetActions() { return _actions; }
    virtual TArray<TObjectPtr<UAction>> GetActions() const { return _actions; }

    /** @return an array of all currently active actions */
    virtual TArray<UAction *> GetActiveActions() { return _activeActions; }
    virtual TArray<UAction *> GetActiveActions() const { return _activeActions; }

protected:
    /** How often actions utilities needs to be updated */
    UPROPERTY(EditAnywhere, Category = "UtilityManager|Evaluator")
    float EvaluationInterval = .1f;

    /** Curve table that contains all unique consideration curves from each action class */
    UPROPERTY(EditDefaultsOnly, Category = "UtilityManager|Evaluator")
    TObjectPtr<class UCurveTable> ScorersCurveTable = nullptr;

    UPROPERTY(EditDefaultsOnly, Instanced, Category = "UtilityManager|Evaluator")
    TSet<class UService *> Services;

    UPROPERTY(EditDefaultsOnly, Category = "UtilityManager|Actions")
    TSet<FActionsPool> ActionsPools;

    /** Action that was exexuting when no other action from pools will selected*/
    UPROPERTY(EditDefaultsOnly, Instanced, Category = "UtilityManager|Actions")
    TObjectPtr<UFallbackAction> FallbackAction = GetMutableDefault<UFallbackAction>();

public:
    virtual TObjectPtr<class UCurveTable> GetCurveTableAsset() { return ScorersCurveTable; }

    UFUNCTION(BlueprintCallable, Category = UtilityManager)
    void SetScorerValue(FGameplayTag InScorerTag, float InValue = 0.f);

    /** Set value for each scorer by tags in container to 0 */
    UFUNCTION(BlueprintCallable, Category = UtilityManager)
    void ResetScorers(FGameplayTagContainer InScorersTags);

    UFUNCTION(BlueprintCallable, Category = UtilityManager)
    void AbortActiveActions();
};