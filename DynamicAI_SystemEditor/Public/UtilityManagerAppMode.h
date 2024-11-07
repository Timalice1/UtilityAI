#pragma once

#include "CoreMinimal.h"
#include "WorkflowOrientedApp/ApplicationMode.h"
#include "WorkflowOrientedApp/WorkflowTabManager.h"

#include "UtilityManagerEditorApp.h"
#include "Tabs/UtilityManagerPrimaryTabFactory.h"
#include "Tabs/UtilityManagerPropertiesTabFactory.h"

/** Application mode for main behavior tree editing mode */
class UtilityManagerAppMode : public FApplicationMode
{
public:
	UtilityManagerAppMode(TSharedPtr<UtilityManagerEditorApp> app)
		: FApplicationMode(TEXT("UtilityManagerAppMode"))
	{
		_app = app;
		_tabs.RegisterFactory(MakeShareable(new UtilityManagerPrimaryTabFactory(app)));
		_tabs.RegisterFactory(MakeShareable(new UtilityManagerPropertiesTabFactory(app)));

		TabLayout = FTabManager::NewLayout("UtilityManagerAppMode_Layout_v1")
						->AddArea(
							FTabManager::NewPrimaryArea()
								->SetOrientation(Orient_Vertical)	
							 	->Split(
									FTabManager::NewSplitter()
										->SetOrientation(Orient_Horizontal)
											->Split (
												FTabManager::NewStack()
													->SetSizeCoefficient(.75f)
													->AddTab(FName(TEXT("UtilityManagerPrimaryTab")), ETabState::OpenedTab) 
											)
											->Split (
												FTabManager::NewStack()
													->SetSizeCoefficient(.25f)
													->AddTab(FName(TEXT("UtilityManagerPropertyTab")), ETabState::OpenedTab)
											)
								)
						);
	};

	virtual void RegisterTabFactories(TSharedPtr<class FTabManager> InTabManager) override
	{
		TSharedPtr<UtilityManagerEditorApp> app = _app.Pin();
		app->PushTabFactories(_tabs);
		FApplicationMode::RegisterTabFactories(InTabManager);
	};
	virtual void PreDeactivateMode() override { FApplicationMode::PreDeactivateMode(); };
	virtual void PostActivateMode() override { FApplicationMode::PostActivateMode(); };

protected:
	TWeakPtr<UtilityManagerEditorApp> _app;
	FWorkflowAllowedTabSet _tabs;
};