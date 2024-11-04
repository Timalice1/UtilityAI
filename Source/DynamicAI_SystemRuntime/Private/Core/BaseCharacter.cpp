#include "Core/BaseCharacter.h"

ABaseCharacter::ABaseCharacter()
{
}

void ABaseCharacter::BeginPlay()
{
    Super::BeginPlay();
    _teamId = FGenericTeamId(TeamID);
}

void ABaseCharacter::GetActorEyesViewPoint(FVector &OutLocation, FRotator &OutRotation) const
{
    if (const auto mesh = GetMesh())
    {
        OutLocation = mesh->GetSocketLocation(CharacterViewPointSocket);
        OutRotation = mesh->GetSocketRotation(CharacterViewPointSocket);
    }
}

bool ABaseCharacter::CanBeSeenFrom(const FVector &ObserverLocation, FVector &OutSeenLocation, int32 &NumberOfLoSChecksPerformed, float &OutSightStrength, const AActor *IgnoreActor, const bool *bWasVisible, int32 *UserData) const
{
    FHitResult _hitRes;
    FVector _sightLocation = GetMesh()->GetSocketLocation(TargetBone);

    bool _hit = GetWorld()->LineTraceSingleByChannel(
        _hitRes,
        ObserverLocation,
        _sightLocation,
        ECC_Visibility,
        FCollisionQueryParams(TEXT("TestPawnLineOfSight"), false, IgnoreActor));

    if (!_hit || (_hitRes.GetActor() &&
                  _hitRes.GetActor()->IsOwnedBy(this)))
    {
        OutSeenLocation = GetMesh()->GetSocketLocation(_hitRes.BoneName);
        OutSightStrength = 1;
        return true;
    }

    OutSightStrength = 0;
    return false;
}   
