#include "Tabs/UtilityManagerPropertiesTabFactory.h"
#include "UtilityManagerEditorApp.h"
#include "Manager.h"
#include "IDetailsView.h"
#include "PropertyEditorModule.h"

TSharedRef<SWidget> UtilityManagerPropertiesTabFactory::CreateTabBody(const FWorkflowTabSpawnInfo &Info) const
{

    TSharedPtr<UtilityManagerEditorApp> app = _app.Pin();
    FPropertyEditorModule &propertyEditorModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>(TEXT("PropertyEditor"));

    FDetailsViewArgs detailsViewArgs;
    {
        detailsViewArgs.bAllowSearch = false;
        detailsViewArgs.bHideSelectionTip = true;
        detailsViewArgs.bLockable = false;
        detailsViewArgs.bSearchInitialKeyFocus = true;
        detailsViewArgs.bUpdatesFromSelection = false;  
        detailsViewArgs.NotifyHook = nullptr;
        detailsViewArgs.bShowOptions = true;
        detailsViewArgs.bShowModifiedPropertiesOption = false;
        detailsViewArgs.bShowScrollBar = false;
    }
    TSharedPtr<IDetailsView> detailsView = propertyEditorModule.CreateDetailView(detailsViewArgs);
    class UManager* managerAsset = app->GetWorkingAsset();
    detailsView->SetObject(managerAsset);

    return SNew(SVerticalBox) + SVerticalBox::Slot()
                                    .FillHeight(1.0f)
                                    .HAlign(HAlign_Fill)
                                        [detailsView.ToSharedRef()];
}
