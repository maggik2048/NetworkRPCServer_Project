// BBHUD.cpp
#include "BBHUD.h"
#include "BBPlayerController.h"
#include "Components/TextBlock.h"
#include "Blueprint/UserWidget.h"

void ABBHUD::BeginPlay()
{
    Super::BeginPlay();

    if (ResultWidgetClass)
    {
        ResultWidget = CreateWidget<UUserWidget>(GetWorld(), ResultWidgetClass);
        if (ResultWidget)
        {
            ResultWidget->AddToViewport();

            // 위젯 내 TextBlock 캐싱
            ResultTextBlock = Cast<UTextBlock>(ResultWidget->GetWidgetFromName(TEXT("ResultText")));
            AttemptsTextBlock = Cast<UTextBlock>(ResultWidget->GetWidgetFromName(TEXT("AttemptsText")));

            // 로컬 플레이어인지 확인
            APlayerController* PC = GetOwningPlayerController();
            if (PC && PC->IsLocalController())
            {
                // 로컬 플레이어 전용 HUD 초기화
                if (ResultTextBlock)
                    ResultTextBlock->SetText(FText::FromString(TEXT("게임 준비 완료!")));
                if (AttemptsTextBlock)
                    AttemptsTextBlock->SetText(FText::FromString(TEXT("[0/3]")));
            }
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