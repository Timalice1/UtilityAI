#pragma once

#include "CoreMinimal.h"
#include "../UtilityManagerEditorApp.h"
#include "WorkflowOrientedApp/WorkflowTabFactory.h"

class UtilityManagerPropertiesTabFactory : public FWorkflowTabFactory
{
public:
    UtilityManagerPropertiesTabFactory(TSharedPtr<UtilityManagerEditorApp> app)
        : FWorkflowTabFactory(FName("UtilityManagerPropertyTab"), app)
    {
        _app = app;
        TabLabel = FText::FromString(TEXT("Details"));
    };

    virtual TSharedRef<SWidget> CreateTabBody(const FWorkflowTabSpawnInfo &Info) const override; 
    virtual FText GetTabToolTipText(const FWorkflowTabSpawnInfo &Info) const override { return FText(); };

protected:
    TWeakPtr<class UtilityManagerEditorApp> _app;
};