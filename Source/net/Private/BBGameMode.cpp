#include "BBGameMode.h"
#include "Net/UnrealNetwork.h"
#include "Engine/Engine.h"

ABBGameMode::ABBGameMode()
{
    // 기본 GameState 설정
    PlayerControllerClass = ABBPlayerController::StaticClass();
    PlayerStateClass = ABBPlayerState::StaticClass();

    GenerateAnswer();
}

void ABBGameMode::GenerateAnswer()
{
    AnswerNumbers.Empty();

    while (AnswerNumbers.Num() < 3)
    {
        int32 Num = FMath::RandRange(1, 9);
        if (!AnswerNumbers.Contains(Num))
            AnswerNumbers.Add(Num);
    }

    FString DebugAnswer = FString::Printf(TEXT("정답: %d%d%d"), AnswerNumbers[0], AnswerNumbers[1], AnswerNumbers[2]);
    if (GEngine)
        GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Green, DebugAnswer);
    UE_LOG(LogTemp, Log, TEXT("%s"), *DebugAnswer);
}

FString ABBGameMode::CheckStrikeBall(const FString& Input)
{
    if (Input.Len() != 3)
        return TEXT("잘못된 입력");

    int32 Strikes = 0;
    int32 Balls = 0;

    for (int32 i = 0; i < 3; i++)
    {
        int32 Num = Input[i] - '0';
        if (Num == AnswerNumbers[i])
            Strikes++;
        else if (AnswerNumbers.Contains(Num))
            Balls++;
    }

    if (Strikes == 3)
        return TEXT("3S"); // 승리
    else if (Strikes == 0 && Balls == 0)
        return TEXT("OUT");
    else
        return FString::Printf(TEXT("%dS%dB"), Strikes, Balls);
}

void ABBGameMode::ProcessPlayerInput(ABBPlayerController* PC, const FString& Input)
{
    if (!PC) return;

    ABBPlayerState* PS = PC->GetPlayerState<ABBPlayerState>();
    if (!PS) return;

    if (PS->CurrentAttempts >= PS->MaxAttempts)
    {
        PC->Client_ShowMessage(TEXT("기회 소진!"));
        return;
    }

    FString Result = CheckStrikeBall(Input);
    PS->CurrentAttempts++;

    // 결과 보내기
    PC->Client_UpdateHUD(Result, PS->CurrentAttempts, PS->MaxAttempts);

    if (Result == TEXT("3S"))
    {
        PS->bIsWinner = true;
        PC->Client_ShowResult(TEXT("승리!"));
        GenerateAnswer(); // 게임 리셋
        for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
        {
            ABBPlayerController* OtherPC = Cast<ABBPlayerController>(*It);
            if (OtherPC && OtherPC != PC)
                OtherPC->Client_ShowResult(TEXT("패배!"));
        }
    }
    else if (PS->CurrentAttempts >= PS->MaxAttempts)
    {
        PC->Client_ShowMessage(TEXT("기회 소진!"));
    }
}


void ABBGameMode::BeginPlay()
{
    Super::BeginPlay();

    if (IsRunningDedicatedServer())
    {
        UE_LOG(LogTemp, Log, TEXT("Dedicated Server started."));
    }
}