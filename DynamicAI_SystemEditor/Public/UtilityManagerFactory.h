#pragma once

#include "CoreMinimal.h"
#include "Manager.h"
#include "UtilityManagerFactory.generated.h"

UCLASS()
class UUtilityManagerFactory : public UFactory
{
    GENERATED_BODY()

public:
    UUtilityManagerFactory()
    {
        SupportedClass = UManager::StaticClass();
    };

public:
    virtual UObject *FactoryCreateNew(UClass *uclass, UObject *inParent, FName name, EObjectFlags flags, UObject *context, FFeedbackContext *warn)
    {
        UManager *managerAsset = NewObject<UManager>(inParent, name, flags);
        return managerAsset;
    };
    
    virtual bool CanCreateNew() const override { return true; };
};