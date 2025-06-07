#include <windows.h>
#include <iostream>

void monitorActivity() {
    LASTINPUTINFO lii;
    lii.cbSize = sizeof(LASTINPUTINFO);
    
    while (true) {
        GetLastInputInfo(&lii);
        DWORD currentTick = GetTickCount();
        DWORD idleTime = currentTick - lii.dwTime;

        if (idleTime < 10000) {
            std::cout << "User is active." << std::endl;
        } else {
            std::cout << "User is inactive." << std::endl;
        }

        // Sleep for 10 seconds before checking again
        std::this_thread::sleep_for(std::chrono::seconds(10));
    }
}
