// Copyright Epic Games, Inc. All Rights Reserved.

#include "DynamicAI_SystemEditor.h"
#include "UtilityManagerAssetAction.h"
#include "IAssetTools.h"
#include "AssetToolsModule.h"
#include "Styling/SlateStyleRegistry.h"
#include "Interfaces/IPluginManager.h"

#define LOCTEXT_NAMESPACE "FDynamicAI_SystemEditorModule"

void FDynamicAI_SystemEditorModule::StartupModule()
{
	IAssetTools &assetToolsModule = IAssetTools::Get();
	EAssetTypeCategories::Type assetType = assetToolsModule.RegisterAdvancedAssetCategory(FName(TEXT("UtilityAI")), LOCTEXT("UtilityAI", "Utility AI"));
	TSharedPtr<UtilityManagerAssetAction> customAssetAction = MakeShareable(new UtilityManagerAssetAction(assetType));
	assetToolsModule.RegisterAssetTypeActions(customAssetAction.ToSharedRef());

	_styleSet = MakeShareable(new FSlateStyleSet(TEXT("UtilityManagerEditorStyle")));
	TSharedPtr<IPlugin> plugin = IPluginManager::Get().FindPlugin("DynamicAI_System");
	FString contentDir = plugin->GetContentDir();
	_styleSet->SetContentRoot(contentDir);

	FSlateImageBrush* thumbnail = new FSlateImageBrush(_styleSet->RootToContentDir(TEXT("UtilityManager"), TEXT(".png")), FVector2D(128.0, 128.0));
	FSlateImageBrush* icon = new FSlateImageBrush(_styleSet->RootToContentDir(TEXT("UtilityManager"), TEXT(".png")), FVector2D(128.0, 128.0));
	_styleSet->Set(TEXT("ClassThumbnail.Manager"), thumbnail);
	_styleSet->Set(TEXT("ClassIcon.Manager"), icon);
	FSlateStyleRegistry::RegisterSlateStyle(*_styleSet);
}

void FDynamicAI_SystemEditorModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	FSlateStyleRegistry::UnRegisterSlateStyle(*_styleSet);

}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FDynamicAI_SystemEditorModule, DynamicAI_SystemEditor)