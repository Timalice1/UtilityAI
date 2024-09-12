#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DynamicAI_Component.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class DYNAMICAI_SYSTEM_API UDynamicAI_Component : public UActorComponent
{
	GENERATED_BODY()

public:
	UDynamicAI_Component();

protected:
	`
};
