#include "UtilityManagerAssetAction.h"
#include "UtilityManagerEditorApp.h"


void UtilityManagerAssetAction::OpenAssetEditor(const TArray<UObject *> &inObjects, TSharedPtr<class IToolkitHost> editWithinLevelEditor)
{
    EToolkitMode::Type mode = editWithinLevelEditor.IsValid() ? EToolkitMode::WorldCentric : EToolkitMode::Standalone;
	for (UObject* object : inObjects) {
		UManager* managerAsset = Cast<UManager>(object);
		if (managerAsset != nullptr) {
			TSharedRef<UtilityManagerEditorApp> editor(new UtilityManagerEditorApp());
			editor->InitEditor(mode, editWithinLevelEditor, managerAsset);
		}
	}
}
