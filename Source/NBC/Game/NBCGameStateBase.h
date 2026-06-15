#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "NBCGameStateBase.generated.h"

UCLASS()
class NBC_API ANBCGameStateBase : public AGameStateBase
{
	GENERATED_BODY()

public:
	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPCBroadcastLoginMessage(const FString& InNameString);
};

