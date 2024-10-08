#include "Debugger/GameplayDebuggerCategory_UtilityAI.h"

#if WITH_GAMEPLAY_DEBUGGER

#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"
#include "Utility/Action.h"
#include "Utility/UtilityManager.h"

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

    if (TObjectPtr<UUtilityManager> manager = controller->GetComponentByClass<UUtilityManager>())
    {
        DataPack.UtilityManager = *manager->GetName();

        if (TObjectPtr<UAction> currentAction = manager->GetCurrentAction())
        {
            DataPack.CurrentAction = currentAction->GetName();
            DataPack.CurrentActionScore = currentAction->GetActionScore();
        }

        for (const TObjectPtr<UAction> &action : manager->GetActions())
            DataPack.AllActions.Add(action->GetName());
    }
}

void FGameplayDebuggerCategory_UtilityAI::DrawData(APlayerController *OwnerPC, FGameplayDebuggerCanvasContext &CanvasContext)
{
    CanvasContext.Printf(TEXT("Controller: [%s]"), *DataPack.ControllerName);
    CanvasContext.Printf(TEXT("Actor: [%s]"), *DataPack.ActorName);

    CanvasContext.Printf(TEXT("\n{green}Utility manager: [%s]\n\tCurrent: {white}[%s] : %f"), 
        *DataPack.UtilityManager, 
        *DataPack.CurrentAction, 
        DataPack.CurrentActionScore);

    for(const FString& action : DataPack.AllActions) 
        CanvasContext.Printf(TEXT("\t%s"), *action);
}

void FGameplayDebuggerCategory_UtilityAI::FRepData::Serialize(FArchive &Ar)
{
    Ar << ActorName;
    Ar << ControllerName;
    Ar << UtilityManager;
    Ar << CurrentAction;
    Ar << CurrentActionScore;
    Ar << AllActions;
}

#endif