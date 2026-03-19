#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "Net/UnrealNetwork.h"
#include "BBPlayerState.generated.h"

UCLASS()
class NET_API ABBPlayerState : public APlayerState
{
    GENERATED_BODY()

public:
    ABBPlayerState();

    // 현재 시도 횟수
    UPROPERTY(Replicated)
    int32 CurrentAttempts;

    // 최대 시도 횟수
    UPROPERTY(Replicated)
    int32 MaxAttempts;

    // 승리 여부
    UPROPERTY(Replicated)
    bool bIsWinner;

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};