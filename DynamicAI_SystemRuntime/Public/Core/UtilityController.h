#pragma once

#include "CoreMinimal.h"
#include "DetourCrowdAIController.h"
#include "Core/UtilityInterface.h"
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
class DYNAMICAI_SYSTEM_API AUtilityController : public ADetourCrowdAIController, public IUtilityInterface
{
    GENERATED_BODY()

public:
    AUtilityController();

    virtual void OnPossess(APawn *InPawn) override;
    virtual FGenericTeamId GetGenericTeamId() const override { return _teamId; };

public:
    virtual void UpdateControlRotation(float DeltaTime, bool bUpdatePawn = true) override;

    virtual FVector GetFocalPointOnActor(const AActor *Actor) const override;
    
    UFUNCTION()
    virtual void BeginPlay() override;

protected:
    UPROPERTY(BlueprintReadWrite, Category = "References")
    TObjectPtr<APawn> _possessedPawn = nullptr;

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Components)
    TObjectPtr<UUtilityManager> UtilityManager;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Components)
    TObjectPtr<UAIPerceptionComponent> AI_Perception;

    UPROPERTY(EditDefaultsOnly, Category = Character)
    float AimHalfHeightOffset = 90.f;

    UPROPERTY(BlueprintReadWrite)
    class ABaseCharacter *_agent = nullptr;

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "AI_Controller")
    TEnumAsByte<ESenseType> CheckSenseType(const FAIStimulus &stimulus) const;

    UUtilityManager *GetUtilityManager_Implementation() override
    {
        return UtilityManager;
    }

private:
    TMap<TSubclassOf<class UAISense>, TEnumAsByte<ESenseType>> _senses;

protected:
    FGenericTeamId _teamId;
    virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor &Other) const override;
};
