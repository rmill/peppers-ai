#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SocketIOLib.h" // Include your Socket.IO library
#include "SocketIOManager.generated.h"

UCLASS()
class YOURPROJECT_API ASocketIOManager : public AActor
{
    GENERATED_BODY()
    
public:    
    // Sets default values for this actor's properties
    ASocketIOManager();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:    
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    // Function to initialize the Socket.IO connection
    void InitializeSocketIO();

    // Function to handle incoming AI prompts
    void OnAIPromptReceived(const FString& Prompt);

    // Function to handle AI responses
    void OnAIResponseReceived(const FString& Response);

    // Function to start the C# Socket.IO server
    void StartCSharpServer();

private:
    // Socket.IO client instance
    TSharedPtr<FSocketIOClient> SocketIOClient;
};
