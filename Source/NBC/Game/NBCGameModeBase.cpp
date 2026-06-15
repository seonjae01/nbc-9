#include "NBCGameModeBase.h"
#include "NBCGameStateBase.h"
#include "Player/NBCPlayerState.h"
#include "Player/NBCPlayerController.h"
#include "EngineUtils.h"

void ANBCGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	SecretNumberString = GenerateSecretNumber();
}

void ANBCGameModeBase::OnPostLogin(AController* NewPlayer)
{
	Super::OnPostLogin(NewPlayer);

	if (ANBCPlayerController* PC = Cast<ANBCPlayerController>(NewPlayer))
	{
		ConnectedPlayers.Add(PC);

		PC->NotificationText = FText::FromString(TEXT("게임 서버에 접속하였습니다."));

		if (ANBCPlayerState* PS = PC->GetPlayerState<ANBCPlayerState>())
		{
			PS->PlayerNameString = FString::Printf(TEXT("Player %d"), ConnectedPlayers.Num());

			if (ANBCGameStateBase* GS = GetGameState<ANBCGameStateBase>())
			{
				GS->MulticastRPCBroadcastLoginMessage(PS->PlayerNameString);
			}
		}
	}
}

void ANBCGameModeBase::ProcessPlayerInput(ANBCPlayerController* InputPlayer, const FString& ChatMessage)
{
	ANBCPlayerState* PS = InputPlayer->GetPlayerState<ANBCPlayerState>();
	if (!PS) return;

	if (IsValidGuess(ChatMessage))
	{
		int32 StrikeCount = 0;
		FString JudgeResult = GetJudgeResult(ChatMessage, StrikeCount);
		PS->CurrentAttempts++;

		FString ChatMessageToBroadcast = FString::Printf(TEXT("%s : %s -> %s%s"),
			*PS->PlayerNameString, *ChatMessage, *JudgeResult, *PS->GetAttemptInfoString());

		for (int32 i = ConnectedPlayers.Num() - 1; i >= 0; --i)
		{
			if (ANBCPlayerController* PC = ConnectedPlayers[i].Get())
			{
				PC->ClientRPCPrintChatMessage(ChatMessageToBroadcast);
			}
		}

		JudgeGame(InputPlayer, StrikeCount);
	}
	else
	{
		FString WarningMessage = TEXT("System: 3자리 숫자를 중복 없이 입력해주세요. (예: 123)");
		InputPlayer->ClientRPCPrintChatMessage(WarningMessage);
	}
}

FString ANBCGameModeBase::GenerateSecretNumber()
{
	TArray<int32> Numbers = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };

	FString Result;
	for (int32 i = 0; i < 3; ++i)
	{
		int32 RandomIndex = FMath::RandRange(0, Numbers.Num() - 1);
		Result.Append(FString::FromInt(Numbers[RandomIndex]));
		Numbers.RemoveAt(RandomIndex);
	}

	return Result;
}

bool ANBCGameModeBase::IsValidGuess(const FString& InputString)
{
	if (InputString.Len() != 3)
		return false;

	TSet<TCHAR> UniqueDigits;
	for (TCHAR C : InputString)
	{
		if (!FChar::IsDigit(C) || C == '0')
			return false;

		UniqueDigits.Add(C);
	}

	if (UniqueDigits.Num() != 3)
		return false;

	return true;
}

FString ANBCGameModeBase::GetJudgeResult(const FString& GuessString, int32& OutStrike)
{
	OutStrike = 0;
	int32 Ball = 0;

	for (int32 i = 0; i < 3; ++i)
	{
		if (SecretNumberString[i] == GuessString[i])
		{
			OutStrike++;
		}
		else if (SecretNumberString.Contains(FString::Printf(TEXT("%c"), GuessString[i])))
		{
			Ball++;
		}
	}

	return OutStrike == 0 && Ball == 0 ? TEXT("OUT") : FString::Printf(TEXT("%dS %dB"), OutStrike, Ball);
}

void ANBCGameModeBase::JudgeGame(ANBCPlayerController* InputPlayer, int32 StrikeCount)
{
	if (StrikeCount == 3)
	{
		ANBCPlayerState* PS = InputPlayer->GetPlayerState<ANBCPlayerState>();
		FString WinMsg = FString::Printf(TEXT("%s 님이 승리하셨습니다."), *PS->PlayerNameString);

		for (int32 i = ConnectedPlayers.Num() - 1; i >= 0; --i)
		{
			if (ANBCPlayerController* PC = ConnectedPlayers[i].Get())
			{
				PC->NotificationText = FText::FromString(WinMsg);
			}
		}

		ResetGame();
	}
	else
	{
		bool bIsDraw = true;
		for (int32 i = ConnectedPlayers.Num() - 1; i >= 0; --i)
		{
			if (ANBCPlayerController* PC = ConnectedPlayers[i].Get())
			{
				if (ANBCPlayerState* PS = PC->GetPlayerState<ANBCPlayerState>())
				{
					if (PS->CurrentAttempts < PS->MaxAttempts)
					{
						bIsDraw = false;
						break;
					}
				}
			}
		}

		if (bIsDraw)
		{
			for (int32 i = ConnectedPlayers.Num() - 1; i >= 0; --i)
			{
				if (ANBCPlayerController* PC = ConnectedPlayers[i].Get())
				{
					PC->NotificationText = FText::FromString(TEXT("무승부..."));
				}
			}

			ResetGame();
		}
	}
}

void ANBCGameModeBase::ResetGame()
{
	SecretNumberString = GenerateSecretNumber();

	for (int32 i = ConnectedPlayers.Num() - 1; i >= 0; --i)
	{
		if (ANBCPlayerController* PC = ConnectedPlayers[i].Get())
		{
			if (ANBCPlayerState* PS = PC->GetPlayerState<ANBCPlayerState>())
			{
				PS->CurrentAttempts = 0;
			}
		}
	}
}
