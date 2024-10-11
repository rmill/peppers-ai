#include "SocketIOManager.h"
#include "SocketIOLib.h"
#include "Misc/Paths.h"
#include "HAL/PlatformProcess.h"
#include "Engine/Engine.h" // For GEngine
#include "Sound/SoundWave.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"

ASocketIOManager::ASocketIOManager()
{
    PrimaryActorTick.bCanEverTick = true;
}

void ASocketIOManager::BeginPlay()
{
    Super::BeginPlay();
    StartCSharpServer();
    InitializeSocketIO();
}

void ASocketIOManager::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ASocketIOManager::InitializeSocketIO()
{
    SocketIOClient = MakeShareable(new FSocketIOClient());
    SocketIOClient->Connect(TEXT("http://localhost:3000"));
    SocketIOClient->On(TEXT("ai_prompt"), this
    {
        OnAIPromptReceived(Prompt);
    });
    SocketIOClient->On(TEXT("ai_response"), this
    {
        OnAIResponseReceived(Response);
    });
}

void ASocketIOManager::OnAIPromptReceived(const FString& Prompt)
{
    UE_LOG(LogTemp, Log, TEXT("Received AI Prompt: %s"), *Prompt);
    // You can send the prompt to the server here if needed
}

void ASocketIOManager::OnAIResponseReceived(const FString& Response)
{
    UE_LOG(LogTemp, Log, TEXT("Received AI Response: %s"), *Response);
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("AI Response: %s"), *Response));
    }

    // Use text-to-speech to play the response
    FString Command = FString::Printf(TEXT("powershell -Command \"Add-Type -AssemblyName System.Speech; $speak = New-Object System.Speech.Synthesis.SpeechSynthesizer; $speak.Speak('%s');\""), *Response);
    FPlatformProcess::CreateProc(*Command, nullptr, true, false, false, nullptr, 0, nullptr, nullptr);
}

void ASocketIOManager::StartCSharpServer()
{
    FString ServerExecutablePath = FPaths::ConvertRelativePathToFull(FPaths::ProjectDir()) + TEXT("Path/To/Your/SocketIOServer.exe"); // Adjust the path to your executable

    FPlatformProcess::CreateProc(*ServerExecutablePath, nullptr, true, false, false, nullptr, 0, nullptr, nullptr);
}
