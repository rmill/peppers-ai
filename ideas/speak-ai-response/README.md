Sure! Here's an updated version of your Unreal Engine script that includes starting a C# Socket.IO server, passing the AI prompt to ChatGPT, and using text-to-speech (TTS) to convert and play the response text as well as display it on the screen.

1. **Create the C# Socket.IO Server**:
    - First, create a C# project and add the `SocketIOSharp`, `System.Net.Http`, and `System.Speech` libraries.

    **Program.cs**:
    ```csharp
    using System;
    using System.Net.Http;
    using System.Text.Json;
    using System.Threading.Tasks;
    using SocketIOSharp.Server;
    using SocketIOSharp.Common;
    using System.Speech.Synthesis;

    class Program
    {
        static async Task Main(string[] args)
        {
            var server = new SocketIOServer(new SocketIOServerOption(3000));
            var synthesizer = new SpeechSynthesizer();

            server.OnConnection += (socket) =>
            {
                Console.WriteLine("A user connected");

                socket.On("ai_prompt", async (data) =>
                {
                    Console.WriteLine("AI Prompt received: " + data);
                    string response = await GetChatGPTResponse(data);
                    Console.WriteLine("AI Response: " + response);
                    synthesizer.SpeakAsync(response);
                    socket.Emit("ai_response", response);
                });

                socket.OnDisconnect += (reason) =>
                {
                    Console.WriteLine("User disconnected: " + reason);
                };
            };

            server.Start();
            Console.WriteLine("Socket.IO server running at http://localhost:3000/");
            Console.ReadLine();
        }

        static async Task<string> GetChatGPTResponse(string prompt)
        {
            string apiKey = "YOUR_API_KEY";
            string apiUrl = "https://api.openai.com/v1/chat/completions";

            using (HttpClient client = new HttpClient())
            {
                client.DefaultRequestHeaders.Add("Authorization", $"Bearer {apiKey}");

                var requestBody = new
                {
                    model = "gpt-3.5-turbo",
                    messages = new[]
                    {
                        new { role = "system", content = "You are a helpful assistant." },
                        new { role = "user", content = prompt }
                    }
                };

                var response = await client.PostAsync(apiUrl, new StringContent(JsonSerializer.Serialize(requestBody), System.Text.Encoding.UTF8, "application/json"));

                if (response.IsSuccessStatusCode)
                {
                    var responseBody = await response.Content.ReadAsStringAsync();
                    dynamic jsonResponse = JsonSerializer.Deserialize<dynamic>(responseBody);
                    return jsonResponse.choices[0].message.content;
                }
                else
                {
                    return "Error: Unable to get response from ChatGPT.";
                }
            }
        }
    }
    ```

2. **Compile the C# Server**:
    - Compile your C# project to generate an executable file (e.g., `SocketIOServer.exe`).

3. **Start the C# Server from Unreal Engine**:
    - Modify your Unreal Engine script to start the C# server executable, handle the AI response, and use text-to-speech to play the response.

    **SocketIOManager.h**:
    ```cpp
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
    ```

    **SocketIOManager.cpp**:
    ```cpp
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
        SocketIOClient->On(TEXT("ai_prompt"), [this](const FString& Prompt)
        {
            OnAIPromptReceived(Prompt);
        });
        SocketIOClient->On(TEXT("ai_response"), [this](const FString& Response)
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
    ```

This setup will start the C# Socket.IO server when your Unreal Engine game begins, listen for AI prompts, pass them to ChatGPT, display the responses on the screen, and use text-to-speech to play the responses. Make sure the path to your C# server executable is correct and replace `"YOUR_API_KEY"` with your actual OpenAI API key.

Let me know if you need any more help with this!

Source: Conversation with Copilot, 10/11/2024
(1) C# Tutorial 86: Text to Speech Converter Sample in C#. https://www.youtube.com/watch?v=9xeIEcr7lqs.
(2) How to Create a Speech To Text Tutorial in C#. https://www.youtube.com/watch?v=VWW3alZQ5EA.
(3) C# Speech Recognition (speech to text) tutorial | Easy and step by step for beginners. https://www.youtube.com/watch?v=caYyjxujZuU.
(4) Speech - Text-To-Speech Synthesis in .NET | Microsoft Learn. https://learn.microsoft.com/en-us/archive/msdn-magazine/2019/june/speech-text-to-speech-synthesis-in-net.
(5) Implementing Text-to-Speech in C# using System.Speech.Synthesis. https://www.codeproject.com/Tips/5357005/Implementing-Text-to-Speech-in-Csharp-using-System.
(6) Using the Text-to-Speech API with C# - Google Codelabs. https://codelabs.developers.google.com/codelabs/cloud-text-speech-csharp/.
