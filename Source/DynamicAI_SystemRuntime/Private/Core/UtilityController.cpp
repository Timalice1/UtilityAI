#include "Core/UtilityController.h"
#include "Core/UtilityManager.h"
#include "Perception/AIPerceptionComponent.h"
#include "Core/BaseCharacter.h"
#include "GameFramework/Pawn.h"
#include "Perception/AISense.h"
#include "Perception/AIPerceptionSystem.h"
#include "Perception/AISense_Sight.h"
#include "Perception/AISense_Damage.h"
#include "Perception/AISense_Hearing.h"
#include "Perception/AISense_Prediction.h"
#include "Perception/AISense_Touch.h"
#include "Perception/AISense_Team.h"
#include "Perception/AISense_Blueprint.h"

AUtilityController::AUtilityController()
{
    UtilityManager = CreateDefaultSubobject<UUtilityManager>("UtilityManagerComponent");
    AI_Perception = CreateDefaultSubobject<UAIPerceptionComponent>("AI_PerceptionComponent");

    _senses.Add(UAISense_Sight::StaticClass(), ESenseType::SightSense);
    _senses.Add(UAISense_Damage::StaticClass(), ESenseType::DamageSense);
    _senses.Add(UAISense_Hearing::StaticClass(), ESenseType::HearingSense);
    _senses.Add(UAISense_Prediction::StaticClass(), ESenseType::PredictionSense);
    _senses.Add(UAISense_Touch::StaticClass(), ESenseType::TouchSense);
    _senses.Add(UAISense_Team::StaticClass(), ESenseType::TeamSense);
    _senses.Add(UAISense_Blueprint::StaticClass(), ESenseType::BlueprintSense);
}

void AUtilityController::OnPossess(APawn *InPawn)
{
    Super::OnPossess(InPawn);

    if (ABaseCharacter *_chr = Cast<ABaseCharacter>(InPawn))
    {
        _agent = _chr;
        _teamId = FGenericTeamId(_agent->TeamID);
    }
}

ETeamAttitude::Type AUtilityController::GetTeamAttitudeTowards(const AActor &Other) const
{
    const APawn *_otherPawn = Cast<APawn>(&Other);
    if (!_otherPawn) /* If perceived actor is not a pawn, mark this actor as neutral*/
        return ETeamAttitude::Neutral;

    const IGenericTeamAgentInterface * _playerTeamInterface = Cast<IGenericTeamAgentInterface>(_otherPawn);
    const IGenericTeamAgentInterface *_aiTeamInterface = Cast<IGenericTeamAgentInterface>(_otherPawn->GetController());
    if(!_playerTeamInterface && !_aiTeamInterface)
        return ETeamAttitude::Neutral;

    FGenericTeamId _otherActorTeamId = NULL;
    if(_aiTeamInterface) 
        _otherActorTeamId = _aiTeamInterface->GetGenericTeamId();
    else if(_playerTeamInterface) 
        _otherActorTeamId = _playerTeamInterface->GetGenericTeamId();

    /*Compare this character team id with other actor team id*/
    FGenericTeamId _thisTeamId = GetGenericTeamId();
    if(_thisTeamId == _otherActorTeamId)
        return ETeamAttitude::Friendly;
    if(_otherActorTeamId == 255)
        return ETeamAttitude::Neutral;

    return ETeamAttitude::Hostile;
}

TEnumAsByte<ESenseType> AUtilityController::CheckSenseType(const FAIStimulus &stimulus) const
{
    TSubclassOf<UAISense> sense = UAIPerceptionSystem::GetSenseClassForStimulus(GetWorld(), stimulus); 
    if(_senses.Contains(sense))
        return *_senses.Find(sense);
    return TEnumAsByte<ESenseType>();
}