#pragma once
#include "../Core/UtilityManager.h"
#include "AIController.h"
#include "Service.generated.h"

UCLASS(MinimalAPI, Abstract, EditInlineNew, Blueprintable, BlueprintType)
class UService : public UObject
{
    GENERATED_BODY()

private:
    UPROPERTY()
    TObjectPtr<class UUtilityManager> _manager;
    UPROPERTY()
    TObjectPtr<class AAIController> _controller;
    UPROPERTY()
    TObjectPtr<class APawn> _pawn;

public: // Parent class overrides
    UService() : Super() {};

    FTimerHandle tickTimer;

    virtual UWorld *GetWorld() const override;

    virtual void PostInitProperties() override;

    virtual void Init(class UUtilityManager *inManager);

    virtual void Execute();
    

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