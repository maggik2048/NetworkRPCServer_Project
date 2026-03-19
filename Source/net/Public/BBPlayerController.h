// BBPlayerController.h
#pragma once
#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BBPlayerController.generated.h"

UCLASS()
class NET_API ABBPlayerController : public APlayerController
{
    GENERATED_BODY()

public:
    void SendInputToServer(const FString& Input);

    UFUNCTION(Server, Reliable)
    void Server_SendInput(const FString& Input);
    void Server_SendInput_Implementation(const FString& Input);

    UFUNCTION(Client, Reliable)
    void Client_UpdateHUD(const FString& Result, int32 Current, int32 Max);

    UFUNCTION(Client, Reliable)
    void Client_ShowResult(const FString& Message);

    UFUNCTION(Client, Reliable)
    void Client_ShowMessage(const FString& Message);

    UFUNCTION(Exec) // 콘솔 입력용
        void InputNumber(const FString& Number);
};