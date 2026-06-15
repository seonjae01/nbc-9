#include "NBCPlayerState.h"
#include "Net/UnrealNetwork.h"

ANBCPlayerState::ANBCPlayerState()
{
	bReplicates = true;
	PlayerNameString = TEXT("None");
	CurrentAttempts = 0;
	MaxAttempts = 3;
}

void ANBCPlayerState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, PlayerNameString);
	DOREPLIFETIME(ThisClass, CurrentAttempts);
	DOREPLIFETIME(ThisClass, MaxAttempts);
}

FString ANBCPlayerState::GetAttemptInfoString()
{
	return FString::Printf(TEXT(" (%d/%d)"), CurrentAttempts, MaxAttempts);
}
