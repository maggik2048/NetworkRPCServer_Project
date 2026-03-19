#pragma once
#include "CoreMinimal.h"

struct FBaseShared
{
    // 중복 없는 3자리 숫자 생성
    static TArray<int32> GenerateRandomNumbers()
    {
        TArray<int32> Numbers;
        while (Numbers.Num() < 3)
        {
            int32 N = FMath::RandRange(1, 9);
            if (!Numbers.Contains(N))
                Numbers.Add(N);
        }
        return Numbers;
    }

    // 입력 유효성 검사
    static bool IsValidInput(const FString& Input)
    {
        if (Input.Len() != 3) return false;

        TSet<TCHAR> CharSet;
        for (TCHAR C : Input)
        {
            if (!FChar::IsDigit(C)) return false;
            if (CharSet.Contains(C)) return false;
            CharSet.Add(C);
        }
        return true;
    }

    // 입력과 정답 비교 (판정)
    static FString CheckStrikeBall(const TArray<int32>& Answer, const FString& Input)
    {
        int32 S = 0, B = 0;
        for (int32 i = 0; i < 3; ++i)
        {
            int32 Num = Input[i] - '0';
            if (Answer[i] == Num) S++;
            else if (Answer.Contains(Num)) B++;
        }
        if (S == 0 && B == 0) return "OUT";
        return FString::Printf(TEXT("%dS%dB"), S, B);
    }
};