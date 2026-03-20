#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BBGameMode.generated.h"

class ABBPlayerController;
class ABBPlayerState;

UCLASS()
class NET_API ABBGameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    ABBGameMode();

    virtual void BeginPlay() override;

    void ProcessPlayerInput(ABBPlayerController* PC, const FString& Input);

private:
    void GenerateAnswer();
    FString CheckStrikeBall(const FString& Input);
    bool IsValidInput(const FString& Input);

private:
    TArray<int32> AnswerNumbers;
};