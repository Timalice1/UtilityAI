#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "UtilityController.generated.h"

class UUtilityManager;
class UAIPerceptionComponent;

UENUM(BlueprintType)
enum ESenseType : uint8
{
    SightSense,
    DamageSense,
    HearingSense,
    PredictionSense,
    TeamSense,
    TouchSense,
    BlueprintSense
};

UCLASS()
class DYNAMICAI_SYSTEM_API AUtilityController : public AAIController
{
    GENERATED_BODY()

public:
    AUtilityController();

    virtual void OnPossess(APawn *InPawn) override;
    virtual FGenericTeamId GetGenericTeamId() const override { return _teamId; };

protected:
    UPROPERTY(BlueprintReadWrite, Category = "References")
    TObjectPtr<APawn> _possessedPawn = nullptr;

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Components)
    TObjectPtr<UUtilityManager> UtilityManager = nullptr;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Components)
    TObjectPtr<UAIPerceptionComponent> AI_Perception = nullptr;

    UPROPERTY(BlueprintReadWrite)
    class ABaseCharacter* _agent = nullptr;

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "AI_Controller")
    TEnumAsByte<ESenseType> CheckSenseType(const FAIStimulus &stimulus) const;

private:
    TMap<TSubclassOf<class UAISense>, TEnumAsByte<ESenseType>> _senses;

protected:
    FGenericTeamId _teamId;
    virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor &Other) const override;
};
