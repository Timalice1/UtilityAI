#pragma once

#include "CoreMinimal.h"
#include "../UtilityManagerEditorApp.h"
#include "WorkflowOrientedApp/WorkflowTabFactory.h"

class UtilityManagerPrimaryTabFactory : public FWorkflowTabFactory
{
public:
    UtilityManagerPrimaryTabFactory(TSharedPtr<UtilityManagerEditorApp> app)
        : FWorkflowTabFactory(FName("UtilityManagerPrimaryTab"), app)
    {
        _app = app;
        TabLabel = FText::FromString(TEXT("Graph"));
    };

    virtual TSharedRef<SWidget> CreateTabBody(const FWorkflowTabSpawnInfo &Info) const override;
    virtual FText GetTabToolTipText(const FWorkflowTabSpawnInfo &Info) const override { return FText(); };

protected:
    TWeakPtr<class UtilityManagerEditorApp> _app;
};