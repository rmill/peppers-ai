#!/bin/bash

# RTMP server URL
RTMP_URL="rtmp://<RaspberryPi_IP>/live/stream"

# Start receiving and playing the stream using ffplay
ffplay -fflags nobuffer -i "$RTMP_URL"
