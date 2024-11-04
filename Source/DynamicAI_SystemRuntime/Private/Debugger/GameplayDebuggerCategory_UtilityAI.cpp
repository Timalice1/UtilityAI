#include "Debugger/GameplayDebuggerCategory_UtilityAI.h"

#if WITH_GAMEPLAY_DEBUGGER

#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"
#include "Utility/Action.h"
#include "Utility/Scorer.h"
#include "Core/UtilityManager.h"

FGameplayDebuggerCategory_UtilityAI::FGameplayDebuggerCategory_UtilityAI()
{
    SetDataPackReplication<FRepData>(&DataPack);
}

void FGameplayDebuggerCategory_UtilityAI::CollectData(APlayerController *OwnerPC, AActor *DebugActor)
{
    if (!DebugActor || !DebugActor->GetInstigatorController())
        return;

    AController *controller = DebugActor->GetInstigatorController();
    if (OwnerPC == Cast<APlayerController>(controller))
        return;

    DataPack.ControllerName = controller->GetName();
    DataPack.ActorName = DebugActor->GetName();

    UUtilityManager *manager = controller->GetComponentByClass<UUtilityManager>();
    if (manager)
    {
        DataPack.UtilityManager = *manager->GetName();
        DataPack.ActiveActions = manager->GetActiveActions();
        DataPack.Pools = manager->GetPools();

        for (UAction *_action : manager->GetActions())
        {
            for (FScorer &_scorer : _action->GetScorers())
            {
                if (!DataPack.Scorers.Contains(_scorer))
                    DataPack.Scorers.Add(_scorer);
            }
        }
    }
}

void FGameplayDebuggerCategory_UtilityAI::DrawData(APlayerController *OwnerPC, FGameplayDebuggerCanvasContext &CanvasContext)
{
    /*-------
    Base info
    -------*/
    CanvasContext.Printf(TEXT("{green}Controller: {yellow}[%s]"), *DataPack.ControllerName);
    CanvasContext.Printf(TEXT("{green}Actor: {yellow}[%s]"), *DataPack.ActorName);
    CanvasContext.Printf(TEXT("{green}Utility manager: {yellow}[%s]"), *DataPack.UtilityManager);

    /*-----
    Scorers
    -----*/
    CanvasContext.Printf(TEXT("\n{green}----------------Scorers---------------------"));
    for (FScorer &_scorer : DataPack.Scorers)
    {
        FString _scorerName = _scorer.ScorerTag.GetTagName().ToString();
        int32 index;
        _scorerName.FindLastChar(TEXT('.'), index);
        _scorerName.RemoveAt(0, index + 1, true);

        CanvasContext.Printf(
            TEXT("{green}[%s]: {yellow}%.4f"),
            *_scorerName,
            _scorer.GetRawScore());
    }

    /*----------
    Active actions
    ----------*/
    CanvasContext.Printf(TEXT("\n{green}----------------Active actions----------------"));
    for (UAction *_action : DataPack.ActiveActions)
    {
        /** Show active actions  */
        CanvasContext.Printf(
            TEXT("\t\t{green}[%s]: {yellow}%.4f"),
            *_action->GetName(),
            _action->GetActionScore());

        /*Show scorers for each active action*/
        for (FScorer &_actionScorer : _action->GetScorers())
        {

            FString _scorerName = _actionScorer.ScorerTag.GetTagName().ToString();
            int32 index;
            _scorerName.FindLastChar(TEXT('.'), index);
            _scorerName.RemoveAt(0, index + 1, true);

            CanvasContext.Printf(
                TEXT("\t\t\t{magenta}[%s%s]: %.5f"),
                _actionScorer.Inverted ? TEXT("NOT ") : TEXT(""),
                *_scorerName,
                _actionScorer.GetScore());
        }
    }

    /*---
    Pools
    ---*/
    CanvasContext.Printf(TEXT("\n{green}--------------------Pools----------------------"));
    for (const FActionsPool &_pool : DataPack.Pools)
    {
        CanvasContext.Printf(TEXT("\t\t{magenta}[%s]:"), *_pool.PoolName.ToString());
        for (const UAction *_action : _pool.GetActions())
        {
            if (_action)
            {
                CanvasContext.Printf(
                    TEXT("\t\t\t{green}[%s]: {yellow}%.4f"),
                    *_action->GetName(),
                    _action->GetActionScore());
            }
        }
    }
}

void FGameplayDebuggerCategory_UtilityAI::FRepData::Serialize(FArchive &Ar)
{
    Ar << ActorName;
    Ar << ControllerName;
    Ar << UtilityManager;
}

#endif