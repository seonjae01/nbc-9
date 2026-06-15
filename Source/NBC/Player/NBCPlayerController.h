#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "NBCPlayerController.generated.h"

UCLASS()
class NBC_API ANBCPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ANBCPlayerController();

	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	void SetChatMessage(const FString& ChatMessage);

	void PrintChatMessage(const FString& ChatMessage);

	UFUNCTION(Server, Reliable)
	void ServerRPCPrintChatMessage(const FString& InputText);

	UFUNCTION(Client, Reliable)
	void ClientRPCPrintChatMessage(const FString& Message);

public:
	UPROPERTY(Replicated, BlueprintReadOnly)
	FText NotificationText;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UUserWidget> ChatInputWidgetClass;

	UPROPERTY()
	TObjectPtr<UUserWidget> ChatInputWidgetInstance;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UUserWidget> NotificationTextWidgetClass;

	UPROPERTY()
	TObjectPtr<UUserWidget> NotificationTextWidgetInstance;
};
