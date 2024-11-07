#pragma once

#include "CoreMinimal.h"
#include "EdGraph/EdGraph.h"
#include "UtilityGraphSchema.generated.h"

UCLASS()
class UUtilityGraphSchema : public UEdGraphSchema
{
    GENERATED_BODY()

public:
    virtual void GetGraphContextActions(FGraphContextMenuBuilder &contextMenuBuilder) const override;
};

USTRUCT()
struct FNewNodeAction : public FEdGraphSchemaAction
{
    GENERATED_BODY()

    FNewNodeAction() {}
    FNewNodeAction(FText inNodeCategory, FText inMenuDescription, FText inTooltip, const int32 inGrouping)
        : FEdGraphSchemaAction(inNodeCategory, inMenuDescription, inTooltip, inGrouping) {}

    virtual UEdGraphNode *PerformAction(UEdGraph *parentGraph, UEdGraphPin *fromPin, const FVector2D location, bool bSelectNewNode = true);
};