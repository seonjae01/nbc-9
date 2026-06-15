#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "NBCGameModeBase.generated.h"

UCLASS()
class NBC_API ANBCGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	virtual void OnPostLogin(AController* NewPlayer) override;

	void ProcessPlayerInput(class ANBCPlayerController* InputPlayer, const FString& ChatMessage);

protected:
	FString SecretNumberString;

	TArray<TWeakObjectPtr<class ANBCPlayerController>> ConnectedPlayers;

private:
	FString GenerateSecretNumber();
	bool IsValidGuess(const FString& InputString);
	FString GetJudgeResult(const FString& GuessString, int32& OutStrike);

	void JudgeGame(ANBCPlayerController* InputPlayer, int32 StrikeCount);
	void ResetGame();
};
