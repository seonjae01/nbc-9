#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "NBCPlayerState.generated.h"

UCLASS()
class NBC_API ANBCPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	ANBCPlayerState();
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	FString GetAttemptInfoString();

public:
	UPROPERTY(Replicated)
	FString PlayerNameString;

	UPROPERTY(Replicated)
	int32 CurrentAttempts;

	UPROPERTY(Replicated)
	int32 MaxAttempts;
};
