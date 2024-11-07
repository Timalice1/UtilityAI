#pragma once

#include "CoreMinimal.h"
#include "AssetTypeActions_Base.h"
#include "Manager.h"

class UtilityManagerAssetAction : public FAssetTypeActions_Base
{
public:
    UtilityManagerAssetAction(EAssetTypeCategories::Type category) {
        _assetCategory = category;
    };

public: // FAssetTypeActions_Base interface
    virtual FText GetName() const override {
        return FText::FromString(TEXT("UtilityManager"));
    };
    virtual FColor GetTypeColor() const override {
         return FColor::Blue;
    };
    virtual UClass *GetSupportedClass() const override {
        return UManager::StaticClass();
    };
    virtual uint32 GetCategories() override {
        return _assetCategory;
    };

    virtual void OpenAssetEditor(const TArray<UObject *> &inObjects, TSharedPtr<class IToolkitHost> editWithinLevelEditor = TSharedPtr<IToolkitHost>()) override;

private:
    EAssetTypeCategories::Type _assetCategory;
};