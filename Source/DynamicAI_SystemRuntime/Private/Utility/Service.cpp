#include "Utility/Service.h"

UWorld *UService::GetWorld() const
{
    if (GIsEditor && !GIsPlayInEditorWorld)
        return nullptr;
    else if (GetOuter())
        return GetOuter()->GetWorld();
    return nullptr;
};

void UService::Init(UUtilityManager *inManager)
{
    if (!IsValid(inManager))
    {
        UE_LOG(UtilityManagerLog, Error, TEXT("[%s]: InManager param is invalid"), *GetName());
        return;
    }
    _manager = inManager;
    /*Initialize controller and controlled pawn references*/
    if (AAIController *Controller = Cast<AAIController>(_manager->GetOwner()))
    {
        _controller = Controller;
        _pawn = _controller->GetPawn();
        Execute();
        return;
    }
    if (APawn *Pawn = Cast<APawn>(_manager->GetOwner()))
    {
        _pawn = Pawn;
        _controller = Cast<AAIController>(_pawn->GetInstigatorController());
        Execute();
        return;
    }
    UE_LOG(UtilityManagerLog, Error, TEXT("[%s]: service is not initialized"), *GetName());
};

void UService::Execute()
{
    if (!_pawn || !_manager)
    {
        UE_CLOG(!_pawn, UtilityManagerLog, Warning, TEXT("[%s]: Pawn property is invalid"), *GetName());
        UE_CLOG(!_manager, UtilityManagerLog, Warning, TEXT("[%s]: Utility Manager property is invalid"), *GetName());
        return;
    }

    this->Start();
    GetOuter()
        ->GetWorld()
        ->GetTimerManager()
        .SetTimer(tickTimer, this, &ThisClass::Update, UpdateInterval, true, 0);
    UE_LOG(UtilityManagerLog, Log, TEXT("Executing service [%s]"), *GetName());
};

void UService::PostInitProperties()
{
    Super::PostInitProperties();
    if (GIsEditor && !GIsPlayInEditorWorld)
        return;
};