// BBPlayerController.cpp
#include "BBPlayerController.h"
#include "BBGameMode.h"
#include "BBPlayerState.h"
#include "BBHUD.h"

void ABBPlayerController::SendInputToServer(const FString& Input)
{
    Server_SendInput(Input);
}

void ABBPlayerController::Server_SendInput_Implementation(const FString& Input)
{
    ABBGameMode* GM = GetWorld()->GetAuthGameMode<ABBGameMode>();
    if (GM)
    {
        GM->ProcessPlayerInput(this, Input);
    }
}

void ABBPlayerController::Client_UpdateHUD_Implementation(const FString& Result, int32 Current, int32 Max)
{
    if (!IsRunningDedicatedServer())
    {
        ABBHUD* HUD = Cast<ABBHUD>(GetHUD());
        if (HUD)
        {
            HUD->UpdateResultText(Result);
            HUD->UpdateAttemptsText(Current, Max);
        }
    }

    UE_LOG(LogTemp, Log, TEXT("[%s] 시도 %d/%d → %s"),
        *GetPlayerState<ABBPlayerState>()->GetPlayerName(),
        Current, Max, *Result);
}

void ABBPlayerController::Client_ShowResult_Implementation(const FString& Message)
{
    if (!IsRunningDedicatedServer())
    {
        ABBHUD* HUD = Cast<ABBHUD>(GetHUD());
        if (HUD)
        {
            HUD->UpdateResultText(Message);
        }
    }
    UE_LOG(LogTemp, Log, TEXT("[%s] 결과: %s"), *GetPlayerState<ABBPlayerState>()->GetPlayerName(), *Message);
}

void ABBPlayerController::Client_ShowMessage_Implementation(const FString& Message)
{
    UE_LOG(LogTemp, Log, TEXT("[%s] 메시지: %s"), *GetPlayerState<ABBPlayerState>()->GetPlayerName(), *Message);
}

void ABBPlayerController::InputNumber(const FString& Number)
{
    SendInputToServer(Number);
}