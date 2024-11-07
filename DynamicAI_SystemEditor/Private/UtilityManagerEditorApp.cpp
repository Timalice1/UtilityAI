#include "UtilityManagerEditorApp.h"
#include "UtilityManagerAppMode.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "UtilityGraphSchema.h"
#include "Manager.h"

void UtilityManagerEditorApp::RegisterTabSpawners(const TSharedRef<class FTabManager>& tabManager) {
    FWorkflowCentricApplication::RegisterTabSpawners(tabManager);
}

void UtilityManagerEditorApp::InitEditor(const EToolkitMode::Type mode, const TSharedPtr<class IToolkitHost>& initToolkitHost, UObject* inObject) {
	TArray<UObject*> objectsToEdit;
    objectsToEdit.Add(inObject);
    
    _workingAsset = Cast<UManager>(inObject);
    _workingGraph = FBlueprintEditorUtils::CreateNewGraph(
        _workingAsset, 
        NAME_None,
        UEdGraph::StaticClass(),
        UUtilityGraphSchema::StaticClass()
    );
     

	InitAssetEditor( 
        mode, 
        initToolkitHost, 
        TEXT("UtilityManagerEditor"), 
        FTabManager::FLayout::NullLayout, 
        true, // createDefaultStandaloneMenu 
        true,  // createDefaultToolbar
        objectsToEdit);

    // Add our modes (just one for this example)
    AddApplicationMode(TEXT("UtilityManagerAppMode"), MakeShareable(new UtilityManagerAppMode(SharedThis(this))));

    // Set the mode
    SetCurrentMode(TEXT("UtilityManagerAppMode"));
}