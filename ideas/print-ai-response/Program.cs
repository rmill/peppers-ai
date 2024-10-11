using System;
using System.Net.Http;
using System.Text.Json;
using System.Threading.Tasks;
using SocketIOSharp.Server;
using SocketIOSharp.Common;

class Program
{
    static async Task Main(string[] args)
    {
        var server = new SocketIOServer(new SocketIOServerOption(3000));

        server.OnConnection += (socket) =>
        {
            Console.WriteLine("A user connected");

            socket.On("ai_prompt", async (data) =>
            {
                Console.WriteLine("AI Prompt received: " + data);
                string response = await GetChatGPTResponse(data);
                Console.WriteLine("AI Response: " + response);
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
