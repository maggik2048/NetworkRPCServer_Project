// BBHUD.h
#pragma once
#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "BBHUD.generated.h"

UCLASS()
class NET_API ABBHUD : public AHUD
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
    TSubclassOf<UUserWidget> ResultWidgetClass;

    UPROPERTY()
    UUserWidget* ResultWidget;

    UTextBlock* ResultTextBlock;
    UTextBlock* AttemptsTextBlock;

    virtual void BeginPlay() override;

    void UpdateResultText(const FString& Message);
    void UpdateAttemptsText(int32 Current, int32 Max);
};