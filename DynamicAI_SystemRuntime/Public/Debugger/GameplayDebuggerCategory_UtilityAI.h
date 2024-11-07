#pragma once

#if WITH_GAMEPLAY_DEBUGGER

#include "GameplayDebuggerCategory.h"

class APlayerController;
class AActor;
class UAction;
class UUtilityManager;

class FGameplayDebuggerCategory_UtilityAI : public FGameplayDebuggerCategory
{
public:
    FGameplayDebuggerCategory_UtilityAI();
    virtual void CollectData(APlayerController *OwnerPC, AActor *DebugActor) override;
    virtual void DrawData(APlayerController *OwnerPC, FGameplayDebuggerCanvasContext &CanvasContext) override;

    static TSharedRef<FGameplayDebuggerCategory> MakeInstance()
    {
        return MakeShareable(new FGameplayDebuggerCategory_UtilityAI());
    };

protected:
    struct FRepData
    {
        FString ControllerName;
        FString ActorName;
        FString UtilityManager;

        TArray<UAction*> ActiveActions;
        TArray<struct FActionsPool> Pools;
        TArray<struct FScorer> Scorers;

        void Serialize(FArchive &ar);
    };

    FRepData DataPack;
};

#endif