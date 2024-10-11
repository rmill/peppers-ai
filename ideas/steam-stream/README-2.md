Yes, it is possible to script the streaming of a game from your host PC to a Raspberry Pi using Steam Link. You can automate the process using scripts and systemd services to ensure that everything starts up and connects automatically. Here’s how you can set it up:

### Automating the Streaming Process

#### 1. Set Up Steam Link on Raspberry Pi

1. **Install Steam Link**:
   - Open a terminal on your Raspberry Pi and run the following commands to install Steam Link:
     ```bash
     sudo apt update
     sudo apt install steamlink
     ```

2. **Create a Script to Launch Steam Link**:
   - Create a script that will launch Steam Link on boot.
   
   **steamlink_start.sh**:
   ```bash
   #!/bin/bash

   # Launch Steam Link
   steamlink
   ```

   - Make the script executable:
     ```bash
     chmod +x /path/to/steamlink_start.sh
     ```

3. **Create a Systemd Service**:
   - Create a systemd service to run the script at boot.

   **/etc/systemd/system/steamlink.service**:
   ```ini
   [Unit]
   Description=Steam Link Service
   After=network.target

   [Service]
   ExecStart=/path/to/steamlink_start.sh
   Restart=always
   User=pi
   Group=pi

   [Install]
   WantedBy=multi-user.target
   ```

   - Enable the service to start on boot:
     ```bash
     sudo systemctl enable steamlink.service
     ```

   - Start the service immediately:
     ```bash
     sudo systemctl start steamlink.service
     ```

#### 2. Automate Game Launch on Host PC

1. **Create a Script to Launch the Game**:
   - On your host PC, create a script to launch the game using Steam's command-line interface.

   **launch_game.sh**:
   ```bash
   #!/bin/bash

   # Steam game ID for your Unreal Engine 5 game
   GAME_ID=YOUR_GAME_ID

   # Launch the game using Steam
   "C:\Program Files (x86)\Steam\steam.exe" -applaunch $GAME_ID
   ```

   - Replace `YOUR_GAME_ID` with the actual Steam game ID of your Unreal Engine 5 game.
   - Make the script executable (if using a Unix-like environment):
     ```bash
     chmod +x /path/to/launch_game.sh
     ```

2. **Create a Scheduled Task (Windows)**:
   - Use Task Scheduler to run the script at startup or at a specific time.

   - Open Task Scheduler and create a new task.
   - Set the trigger to "At startup" or your preferred schedule.
   - Set the action to "Start a program" and browse to your script.

### Summary

By setting up these scripts and services, you can automate the process of starting Steam Link on your Raspberry Pi and launching your game on the host PC. This way, you can stream your game without needing to manually start the process each time.

Let me know if you need any more help with this!
