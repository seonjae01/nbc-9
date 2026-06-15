#include "NBCPlayerController.h"
#include "Game/NBCGameModeBase.h"
#include "NBCPlayerState.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "EngineUtils.h"
#include "NBC.h"

ANBCPlayerController::ANBCPlayerController()
{
	bReplicates = true;
}

void ANBCPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (!IsLocalPlayerController())
		return;

	FInputModeUIOnly InputModeUIOnly;
	SetInputMode(InputModeUIOnly);
	bShowMouseCursor = true;

	if (IsValid(ChatInputWidgetClass))
	{
		ChatInputWidgetInstance = CreateWidget<UUserWidget>(this, ChatInputWidgetClass);
		if (IsValid(ChatInputWidgetInstance))
		{
			ChatInputWidgetInstance->AddToViewport();
		}
	}

	if (IsValid(NotificationTextWidgetClass))
	{
		NotificationTextWidgetInstance = CreateWidget<UUserWidget>(this, NotificationTextWidgetClass);
		if (IsValid(NotificationTextWidgetInstance))
		{
			NotificationTextWidgetInstance->AddToViewport();
		}
	}
}

void ANBCPlayerController::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ThisClass, NotificationText);
}

void ANBCPlayerController::SetChatMessage(const FString& ChatMessage)
{
	if (IsLocalController())
	{
		ServerRPCPrintChatMessage(ChatMessage);
	}
}

void ANBCPlayerController::PrintChatMessage(const FString& ChatMessage)
{
	ChatingFunctionLibrary::PrintString(this, ChatMessage, 10.f);
}

void ANBCPlayerController::ServerRPCPrintChatMessage_Implementation(const FString& Message)
{
	if (ANBCGameModeBase* GM = Cast<ANBCGameModeBase>(UGameplayStatics::GetGameMode(this)))
	{
		GM->ProcessPlayerInput(this, Message);
	}
}

void ANBCPlayerController::ClientRPCPrintChatMessage_Implementation(const FString& Message)
{
	PrintChatMessage(Message);
}
