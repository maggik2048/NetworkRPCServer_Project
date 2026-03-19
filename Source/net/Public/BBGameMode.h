#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BBPlayerController.h"
#include "BBPlayerState.h"
#include "BBGameMode.generated.h"

UCLASS()
class NET_API ABBGameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    ABBGameMode();

    UPROPERTY()
    TArray<int32> AnswerNumbers;

    // 게임 시작 시 정답 생성
    void GenerateAnswer();

    // 입력 처리
    void ProcessPlayerInput(ABBPlayerController* PC, const FString& Input);

    // 여기에 추가: BeginPlay 선언
    virtual void BeginPlay() override;

private:
    FString CheckStrikeBall(const FString& Input);
};