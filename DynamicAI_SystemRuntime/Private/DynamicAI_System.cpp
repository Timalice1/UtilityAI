// Copyright Epic Games, Inc. All Rights Reserved.

#include "DynamicAI_System.h"

#if WITH_GAMEPLAY_DEBUGGER
#include "GameplayDebugger.h"
#include "Debugger/GameplayDebuggerCategory_UtilityAI.h"
#endif

#define LOCTEXT_NAMESPACE "FDynamicAI_SystemModule"


void FDynamicAI_SystemModule::StartupModule()
{
// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
#if WITH_GAMEPLAY_DEBUGGER
	IGameplayDebugger &GameplayDebuggerModule = IGameplayDebugger::Get();
	GameplayDebuggerModule.RegisterCategory(
		"UtilityAI",
		IGameplayDebugger::FOnGetCategory::CreateStatic(
			&FGameplayDebuggerCategory_UtilityAI::MakeInstance),
		EGameplayDebuggerCategoryState::EnabledInGameAndSimulate,
		6);
	GameplayDebuggerModule.NotifyCategoriesChanged();
#endif //WITH_GAMEPLAY_DEBUGGER
}

void FDynamicAI_SystemModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

#if WITH_GAMEPLAY_DEBUGGER
	if (IGameplayDebugger::IsAvailable())
	{
		IGameplayDebugger &GameplayDebuggerModule = IGameplayDebugger::Get();
		GameplayDebuggerModule.UnregisterCategory("UtilityAI");
		GameplayDebuggerModule.NotifyCategoriesChanged();
	}
#endif
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FDynamicAI_SystemModule, DynamicAI_System)
