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

    const FGameplayDebuggerInputHandlerConfig DetailsConfig(TEXT("ToggleDetails"), TEXT("Divide"));
    const FGameplayDebuggerInputHandlerConfig ActiveActionsConfig(TEXT("ToogleActiveActions"), TEXT("Multiply"));

    BindKeyPress(DetailsConfig, this, &FGameplayDebuggerCategory_UtilityAI::ToggleDetailView);
    BindKeyPress(ActiveActionsConfig, this, &FGameplayDebuggerCategory_UtilityAI::ToggleActiveActions);
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
            if(!_action)
                continue;
            for (const FScorer &_scorer : _action->GetScorers())
            {
                if (!DataPack.Scorers.Contains(_scorer))
                    DataPack.Scorers.Add(_scorer);
            }
        }
    }
}

void FGameplayDebuggerCategory_UtilityAI::DrawData(APlayerController *OwnerPC, FGameplayDebuggerCanvasContext &CanvasContext)
{

    _posX = CanvasContext.DefaultX;
    /*-------
    Base info
    -------*/
    CanvasContext.Printf(TEXT("{green}Controller: {yellow}[%s]"), *DataPack.ControllerName);
    CanvasContext.Printf(TEXT("{green}Actor: {yellow}[%s]"), *DataPack.ActorName);
    CanvasContext.Printf(TEXT("{green}Utility manager: {yellow}[%s]"), *DataPack.UtilityManager);

    if (DataPack.Scorers.Num() > 1)
        CanvasContext.Print(FString::Printf(TEXT("{green}Press {yellow}[%s]{green} to show details"), *GetInputHandlerDescription(0)));
    CanvasContext.Print(FString::Printf(TEXT("{green}Press {yellow}[%s]{green} to show active actions"), *GetInputHandlerDescription(1)));

    /*-----
    Scorers
    -----*/
    CanvasContext.Printf(TEXT("\n{green}----------------Scorers---------------------"));
    for (FScorer &_scorer : DataPack.Scorers)
    {
        CanvasContext.Printf(
            TEXT("{green}[%s]: {yellow}%.4f"),
            *_scorer.ScorerTag.GetTagName().ToString(),
            _scorer.GetRawScore());
    }

    /*----------
    Active actions
    ----------*/
    if (bShowActiveActions)
    {

        CanvasContext.Printf(TEXT("\n{green}----------------Active actions----------------"));
        for (UAction *_action : DataPack.ActiveActions)
        {
            /** Show active actions  */
            CanvasContext.Printf(
                TEXT("\t\t{green}[%s]: {yellow}%.4f"),
                *_action->GetName(),
                _action->GetActionScore());
            ShowScorers(_action, CanvasContext);
        }
    }

    /*---
    Pools
    ---*/
    CanvasContext.Printf(TEXT("\n{green}--------------------Pools----------------------"));
    for (const FActionsPool &_pool : DataPack.Pools)
    {
        CanvasContext.CursorX = _posX;
        CanvasContext.Printf(TEXT("\t\t{magenta}[%s]:"), *_pool.PoolName.ToString());
        for (UAction *_action : _pool.GetActions())
        {
            if (_action)
            {
                CanvasContext.CursorX = _posX;
                CanvasContext.Printf(
                    TEXT("\t\t\t{green}[%s]: {yellow}%.4f"),
                    *_action->GetName(),
                    _action->GetActionScore());
                ShowScorers(_action, CanvasContext);
            }
        }
    }
}

void FGameplayDebuggerCategory_UtilityAI::ShowScorers(UAction *_action, FGameplayDebuggerCanvasContext &CanvasContext)
{
    if (!bShowDetails)
        return;
    int i = 0;

    for (FScorer &_actionScorer : _action->GetScorers())
    {

        CanvasContext.CursorX = _posX;
        CanvasContext.Printf(
            TEXT("\t\t\t\t{magenta}[%s%s%s]: %.5f"),
            i == 0 ? TEXT("") : *(UEnum::GetValueAsString(_actionScorer.Operator) + FString(TEXT(" "))),
            _actionScorer.Inverted ? TEXT("NOT ") : TEXT(""),
            *_actionScorer.ScorerTag.GetTagName().ToString(),
            _actionScorer.GetScore());
        ++i;
        if(CanvasContext.CursorY > 1000){
            _posX += 400;
            CanvasContext.CursorY = CanvasContext.DefaultY;
        }
        
    }
}

void FGameplayDebuggerCategory_UtilityAI::FRepData::Serialize(FArchive &Ar)
{
    Ar << ActorName;
    Ar << ControllerName;
    Ar << UtilityManager;
}

void FGameplayDebuggerCategory_UtilityAI::ToggleDetailView()
{
    bShowDetails = !bShowDetails;
}
void FGameplayDebuggerCategory_UtilityAI::ToggleActiveActions()
{
    bShowActiveActions = !bShowActiveActions;
}

#endif
