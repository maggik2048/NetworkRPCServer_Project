#include "BBGameMode.h"
#include "BBPlayerController.h"
#include "BBPlayerState.h"
#include "Engine/Engine.h"

ABBGameMode::ABBGameMode()
{
    PlayerControllerClass = ABBPlayerController::StaticClass();
    PlayerStateClass = ABBPlayerState::StaticClass();

    GenerateAnswer();
}

void ABBGameMode::BeginPlay()
{
    Super::BeginPlay();

    if (IsRunningDedicatedServer())
    {
        UE_LOG(LogTemp, Log, TEXT("Dedicated Server started."));
    }
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

    FString DebugAnswer = FString::Printf(TEXT("정답: %d%d%d"),
        AnswerNumbers[0], AnswerNumbers[1], AnswerNumbers[2]);

    if (GEngine)
        GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Green, DebugAnswer);

    UE_LOG(LogTemp, Log, TEXT("%s"), *DebugAnswer);
}

bool ABBGameMode::IsValidInput(const FString& Input)
{
    if (Input.Len() != 3)
        return false;

    TSet<TCHAR> UniqueDigits;

    for (int32 i = 0; i < 3; i++)
    {
        TCHAR Char = Input[i];

        if (!FChar::IsDigit(Char))
            return false;

        if (Char == '0')
            return false;

        if (UniqueDigits.Contains(Char))
            return false;

        UniqueDigits.Add(Char);
    }

    return true;
}

FString ABBGameMode::CheckStrikeBall(const FString& Input)
{
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
        return TEXT("3S");
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

    // ✅ 입력 검증
    if (!IsValidInput(Input))
    {
        PC->Client_ShowMessage(TEXT("다시 입력하세요 (3자리, 중복 없음, 숫자만)"));
        return;
    }

    // ✅ 기회 체크
    if (PS->CurrentAttempts >= PS->MaxAttempts)
    {
        PC->Client_ShowMessage(TEXT("기회 소진!"));
        return;
    }

    FString Result = CheckStrikeBall(Input);

    PS->CurrentAttempts++;

    PC->Client_UpdateHUD(Result, PS->CurrentAttempts, PS->MaxAttempts);

    // 🎉 승리
    if (Result == TEXT("3S"))
    {
        PS->bIsWinner = true;
        PC->Client_ShowResult(TEXT("승리!"));

        // 다른 플레이어 패배 처리
        for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
        {
            ABBPlayerController* OtherPC = Cast<ABBPlayerController>(*It);
            if (OtherPC && OtherPC != PC)
            {
                OtherPC->Client_ShowResult(TEXT("패배!"));
            }
        }

        // 게임 리셋
        GenerateAnswer();

        for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
        {
            ABBPlayerController* OtherPC = Cast<ABBPlayerController>(*It);
            if (OtherPC)
            {
                ABBPlayerState* OtherPS = OtherPC->GetPlayerState<ABBPlayerState>();
                if (OtherPS)
                {
                    OtherPS->CurrentAttempts = 0;
                    OtherPS->bIsWinner = false;
                }
            }
        }
    }
    else if (PS->CurrentAttempts >= PS->MaxAttempts)
    {
        PC->Client_ShowMessage(TEXT("기회 소진!"));
    }
}