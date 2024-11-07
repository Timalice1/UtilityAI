#pragma once

#include "CoreMinimal.h"
#include "WorkflowOrientedApp/WorkflowCentricApplication.h"

class UtilityManagerEditorApp : public FWorkflowCentricApplication, public FEditorUndoClient, public FNotifyHook
{
public: // FWorkflowCentricApplication interface
    virtual void RegisterTabSpawners(const TSharedRef<class FTabManager> &TabManager) override;
    void InitEditor(const EToolkitMode::Type mode, const TSharedPtr<class IToolkitHost> &initToolkitHost, UObject *inObject);

    class UManager *GetWorkingAsset() { return _workingAsset; }

    class UEdGraph *GetWorkingGraph() { return _workingGraph; }

public: // FAssetEditorToolkit interface
    virtual FName GetToolkitFName() const override { return FName(TEXT("UtilityManagerEditorApp")); }
    virtual FText GetBaseToolkitName() const override { return FText::FromString(TEXT("UtilityManagerEditorApp")); }
    virtual FString GetWorldCentricTabPrefix() const override { return TEXT("UtilityManagerEditorApp"); }
    virtual FLinearColor GetWorldCentricTabColorScale() const override { return FLinearColor(0.3f, 0.2f, 0.5f, 0.5f); }
    virtual FString GetDocumentationLink() const override { return TEXT("https://google.com"); }
    virtual void OnToolkitHostingStarted(const TSharedRef<class IToolkit> &Toolkit) override {}
    virtual void OnToolkitHostingFinished(const TSharedRef<class IToolkit> &Toolkit) override {}

private:
    class UManager *_workingAsset = nullptr;

    class UEdGraph* _workingGraph = nullptr;
};