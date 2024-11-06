#pragma once

#include "Perception/AISightTargetInterface.h"
#include "GameFramework/Character.h"
#include "GenericTeamAgentInterface.h"
#include "BaseCharacter.generated.h"

UCLASS()
class ABaseCharacter : public ACharacter,
                       public IAISightTargetInterface,
                       public IGenericTeamAgentInterface
{
    GENERATED_BODY()

public:
    ABaseCharacter();

    virtual void BeginPlay() override;

    virtual void GetActorEyesViewPoint(FVector &OutLocation, FRotator &OutRotation) const override;

    virtual bool CanBeSeenFrom(
        const FVector &ObserverLocation,
        FVector &OutSeenLocation,
        int32 &NumberOfLoSChecksPerformed,
        float &OutSightStrength,
        const AActor *IgnoreActor = nullptr,
        const bool *bWasVisible = nullptr,
        int32 *UserData = nullptr) const override;

    virtual FGenericTeamId GetGenericTeamId() const override { return _teamId; };

public:
    UPROPERTY(EditAnywhere, Category = "Character")
    int32 TeamID = 0;

    virtual FName GetTargetBone() const { return TargetBone; }

protected:
    UPROPERTY(EditDefaultsOnly, Category = Character)
    FName CharacterViewPointSocket = TEXT("head");

    UPROPERTY(EditDefaultsOnly, Category = Character)
    FName TargetBone = TEXT("spine_03");

private:
    FGenericTeamId _teamId;
};