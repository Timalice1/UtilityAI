// Copyright Epic Games, Inc. All Rights Reserved.

#include "DynamicAI_System.h"

#define LOCTEXT_NAMESPACE "FDynamicAI_SystemModule"

void FDynamicAI_SystemModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
}

void FDynamicAI_SystemModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FDynamicAI_SystemModule, DynamicAI_System)