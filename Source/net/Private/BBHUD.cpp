// BBHUD.cpp
#include "BBHUD.h"
#include "Engine/World.h"

void ABBHUD::BeginPlay()
{
    Super::BeginPlay();

    if (!IsRunningDedicatedServer() && ResultWidgetClass)
    {
        ResultWidget = CreateWidget<UUserWidget>(GetWorld(), ResultWidgetClass);
        if (ResultWidget)
        {
            ResultWidget->AddToViewport();
            ResultTextBlock = Cast<UTextBlock>(ResultWidget->GetWidgetFromName(TEXT("ResultText")));
            AttemptsTextBlock = Cast<UTextBlock>(ResultWidget->GetWidgetFromName(TEXT("AttemptsText")));
        }
    }
}

void ABBHUD::UpdateResultText(const FString& Message)
{
    if (!IsRunningDedicatedServer() && ResultTextBlock)
    {
        ResultTextBlock->SetText(FText::FromString(Message));
    }
}

void ABBHUD::UpdateAttemptsText(int32 Current, int32 Max)
{
    if (!IsRunningDedicatedServer() && AttemptsTextBlock)
    {
        FString AttemptsStr = FString::Printf(TEXT("[%d / %d]"), Current, Max);
        AttemptsTextBlock->SetText(FText::FromString(AttemptsStr));
    }
}