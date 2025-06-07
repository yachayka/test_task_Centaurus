#include <windows.h>
#include <iostream>
#include <fstream>
#include <string>
#include <thread>
#include <chrono>
#include <ctime>
#include "client.h"

using namespace std;

void sendHeartbeat(const string& serverAddress) {
    // This function sends heartbeat to the server (for simplicity, assume it's a simple text message).
    // You can use sockets or HTTP here (raw TCP sockets in this example).
    
    // Placeholder for actual networking code
    cout << "Sending heartbeat to server: " << serverAddress << endl;
}

void monitorActivity() {
    LASTINPUTINFO lii;
    lii.cbSize = sizeof(LASTINPUTINFO);
    
    while (true) {
        GetLastInputInfo(&lii);
        DWORD currentTick = GetTickCount();
        DWORD idleTime = currentTick - lii.dwTime;

        if (idleTime < 10000) {
            cout << "User is active." << endl;
        } else {
            cout << "User is inactive." << endl;
        }

        this_thread::sleep_for(chrono::seconds(10)); // check every 10 seconds
    }
}

void captureScreenshot() {
    // Call the function to capture the screen (see screenshot.cpp)
    CaptureScreen();
}

int main() {
    string serverAddress = "http://localhost:5000";  // Change this to your server address

    // Start activity monitoring in a separate thread
    thread activityThread(monitorActivity);

    // Periodically send heartbeat to server
    thread heartbeatThread(sendHeartbeat, serverAddress);

    // Start capturing screenshots periodically
    thread screenshotThread(captureScreenshot);

    activityThread.join();
    heartbeatThread.join();
    screenshotThread.join();

    return 0;
}
