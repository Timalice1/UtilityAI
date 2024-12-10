#pragma once
#include "../Core/UtilityManager.h"
#include "AIController.h"
#include "Service.generated.h"

class APawn;
class UUtilityManager;
class AAIController;

UCLASS(MinimalAPI, Abstract, EditInlineNew, Blueprintable, BlueprintType)
class UService : public UObject
{
    GENERATED_BODY()

private:
    TObjectPtr<UUtilityManager> _manager;
    TObjectPtr<AAIController> _controller;
    TObjectPtr<APawn> _pawn;

protected: 

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "UtilitySystem|Service")
    APawn *GetPawn() { return _pawn;}
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "UtilitySystem|Service")
    UUtilityManager *GetUtilityManager() { return _manager; };
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "UtilitySystem|Service")
    AAIController *GetController() { return _controller; };

public: // Parent class overrides
    UService() : Super() {};

    FTimerHandle tickTimer;

    virtual UWorld *GetWorld() const override;

    virtual void PostInitProperties() override;

    virtual void Init(class UUtilityManager *inManager);

    virtual void Activate();
    virtual void Deactivate();
    

protected: // Properties
    UPROPERTY(EditDefaultsOnly, Category = "ServiceConfig")
    float UpdateInterval = .01f;

protected: // Events
    UFUNCTION(BlueprintImplementableEvent, Category = "UtilityAI|Service")
    void ServiceStart(APawn *Pawn, UUtilityManager *Manager);
    virtual void Start()
    {
        ServiceStart(_pawn, _manager);
    };

    UFUNCTION(BlueprintImplementableEvent, Category = "UtilityAI|Service")
    void ServiceUpdate(APawn *Pawn, UUtilityManager *Manager);
    virtual void Update()
    {
        ServiceUpdate(_pawn, _manager);
    };
};