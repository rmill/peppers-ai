Got it! Let's create a script for the Raspberry Pi that will receive and play the 4K video stream. We'll use `ffplay`, which is part of the FFmpeg suite, to play the incoming stream.

### 1. Create the Receiving Script

First, create a script that will handle receiving and playing the video stream.

**receive_stream.sh**:
```bash
#!/bin/bash

# RTMP server URL
RTMP_URL="rtmp://<RaspberryPi_IP>/live/stream"

# Start receiving and playing the stream using ffplay
ffplay -fflags nobuffer -i "$RTMP_URL"
```
- Replace `<RaspberryPi_IP>` with the IP address of your Raspberry Pi.

### 2. Make the Script Executable

Make the script executable by running the following command:
```bash
chmod +x /path/to/receive_stream.sh
```

### 3. Create a Systemd Service

Create a systemd service to run the script at boot.

**/etc/systemd/system/receive_stream.service**:
```ini
[Unit]
Description=4K Video Receiving Service
After=network.target

[Service]
ExecStart=/path/to/receive_stream.sh
Restart=always
User=pi
Group=pi

[Install]
WantedBy=multi-user.target
```
- Replace `/path/to/receive_stream.sh` with the path to your script.
- Ensure the `User` and `Group` are set to the user running the script (typically `pi`).

### 4. Enable the Service

Enable the service to start on boot:
```bash
sudo systemctl enable receive_stream.service
```

### 5. Start the Service

Start the service immediately:
```bash
sudo systemctl start receive_stream.service
```

### Summary

This setup will ensure that your Raspberry Pi starts receiving and playing the 4K video stream as soon as it boots up. The `systemd` service will manage the script, ensuring it restarts if it fails.

Let me know if you need any more help with this!
