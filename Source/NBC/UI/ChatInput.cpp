#include "ChatInput.h"
#include "Components/EditableTextBox.h"
#include "Player/NBCPlayerController.h"

void UChatInput::NativeConstruct()
{
	Super::NativeConstruct();

	if (!EditableTextBox_ChatInput->OnTextCommitted.IsAlreadyBound(this, &ThisClass::OnChatInputTextCommitted))
	{
		EditableTextBox_ChatInput->OnTextCommitted.AddDynamic(this, &ThisClass::OnChatInputTextCommitted);
	}
}

void UChatInput::NativeDestruct()
{
	Super::NativeDestruct();

	if (EditableTextBox_ChatInput->OnTextCommitted.IsAlreadyBound(this, &ThisClass::OnChatInputTextCommitted))
	{
		EditableTextBox_ChatInput->OnTextCommitted.RemoveDynamic(this, &ThisClass::OnChatInputTextCommitted);
	}
}

void UChatInput::OnChatInputTextCommitted(const FText& Text, ETextCommit::Type CommitMethod)
{
	if (CommitMethod == ETextCommit::OnEnter)
	{
		if (APlayerController* OwningPlayerController = GetOwningPlayer())
		{
			if (ANBCPlayerController* OwningPC = Cast<ANBCPlayerController>(OwningPlayerController))
			{
				OwningPC->SetChatMessage(Text.ToString());
				EditableTextBox_ChatInput->SetText(FText());
			}
		}
	}
}
