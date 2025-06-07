# WorkMonitor - Client-Server Monitoring Application

## Overview

This project contains a simple client-server application to monitor user activity on a network of computers. The client runs silently on a Windows machine, tracking user input activity, sending heartbeats, and capturing screenshots upon request from the server.

## Client-side (C/C++)

The client performs the following tasks:
- Monitor user activity (idle time and activity status)
- Send heartbeat data to the server
- Capture screenshots of the desktop

### Dependencies

- C++ compiler (e.g., Visual Studio or GCC on Windows)
- Windows SDK (for accessing Windows APIs like `GetLastInputInfo`)

### How to Build

1. Clone the repository:
   ```bash
   git clone https://github.com/yachayka/WorkMonitor.git
