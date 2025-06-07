#ifndef CLIENT_H
#define CLIENT_H

#include <string>

// Function declarations
void sendHeartbeat(const std::string& serverAddress);
void monitorActivity();
void captureScreenshot();

#endif // CLIENT_H
