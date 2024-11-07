#include "Tabs/UtilityManagerPrimaryTabFactory.h"
#include "UtilityManagerEditorApp.h"
#include "Manager.h"
#include "IDetailsView.h"
#include "PropertyEditorModule.h"
#include "GraphEditor.h"
#include "Editor/UnrealEd/Public/Kismet2/BlueprintEditorUtils.h"
#include "Kismet2/KismetEditorUtilities.h"

TSharedRef<SWidget> UtilityManagerPrimaryTabFactory::CreateTabBody(const FWorkflowTabSpawnInfo &Info) const
{
    TSharedPtr<UtilityManagerEditorApp> app = _app.Pin();

    return SNew(SVerticalBox) 
        + SVerticalBox::Slot()
            .FillHeight(1.0f)
            .HAlign(HAlign_Fill)
                [
                    SNew(SGraphEditor)
                        .IsEditable(true)
                        .GraphToEdit(app->GetWorkingGraph())   
                ];
}
