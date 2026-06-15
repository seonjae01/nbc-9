#include "NBCGameStateBase.h"
#include "Player/NBCPlayerController.h"
#include "Kismet/GameplayStatics.h"

void ANBCGameStateBase::MulticastRPCBroadcastLoginMessage_Implementation(const FString& InNameString)
{
	if (GetNetMode() != NM_DedicatedServer)
	{
		APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (IsValid(PC))
		{
			ANBCPlayerController* NBCPC = Cast<ANBCPlayerController>(PC);
			if (IsValid(NBCPC))
			{
				FString NotificationString = InNameString + TEXT(" 님이 게임에 참여했습니다.");
				NBCPC->ClientRPCPrintChatMessage(NotificationString);
			}
		}
	}
}
