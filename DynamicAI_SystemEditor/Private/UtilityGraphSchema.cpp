#include "UtilityGraphSchema.h"

void UUtilityGraphSchema::GetGraphContextActions(FGraphContextMenuBuilder &contextMenuBuilder) const
{
    TSharedPtr<FNewNodeAction> newNodeAction(
        new FNewNodeAction(
            FText::FromString(TEXT("Nodes")),
            FText::FromString(TEXT("NewAction")),
            FText::FromString(TEXT("Tooltip")),
            0));

    contextMenuBuilder.AddAction(newNodeAction);
}

UEdGraphNode *FNewNodeAction::PerformAction(UEdGraph *parentGraph, UEdGraphPin *fromPin, const FVector2D location, bool bSelectNewNode)
{
    UEdGraphNode *result = NewObject<UEdGraphNode>(parentGraph);
    result->NodePosX = location.X;
    result->NodePosY = location.Y;

    parentGraph->Modify();
    parentGraph->AddNode(result, true, true);

    return result;
}
