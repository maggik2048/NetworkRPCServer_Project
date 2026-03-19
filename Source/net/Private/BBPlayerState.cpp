#include "BBPlayerState.h"

ABBPlayerState::ABBPlayerState()
{
    CurrentAttempts = 0;
    MaxAttempts = 3;
    bIsWinner = false;
}

void ABBPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ABBPlayerState, CurrentAttempts);
    DOREPLIFETIME(ABBPlayerState, MaxAttempts);
    DOREPLIFETIME(ABBPlayerState, bIsWinner);
}