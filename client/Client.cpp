#include <iostream>
#include <winsock2.h>
#include <windows.h>
#include <string>
#include <thread>
#include <chrono>
#include <ctime>
#include <fstream>
#include <gdiplus.h>

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "gdiplus.lib")

using namespace std;
using namespace Gdiplus;

const char* SERVER_IP = "127.0.0.1";  // Server IP
const int SERVER_PORT = 8080;          // Server port
const string USERNAME = "JohnDoe";     // Client username

// Initialize Winsock
bool initializeWinsock() {
    WSADATA wsaData;
    return WSAStartup(MAKEWORD(2, 2), &wsaData) == 0;
}

// Create and connect socket to the server
SOCKET createSocket() {
    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) {
        cerr << "Socket creation failed!" << endl;
        return INVALID_SOCKET;
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    serverAddr.sin_addr.s_addr = inet_addr(SERVER_IP);

    if (connect(sock, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        cerr << "Connection to server failed!" << endl;
        closesocket(sock);
        return INVALID_SOCKET;
    }

    return sock;
}

// Function to capture the screenshot and save it as a file
void CaptureScreenshot(const string& filename) {
    // Initialize GDI+
    ULONG_PTR gdiplusToken;
    GdiplusStartupInput gdiplusStartupInput;
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    // Get the screen device context
    HDC hScreenDC = GetDC(NULL);
    HDC hMemoryDC = CreateCompatibleDC(hScreenDC);

    // Get screen width and height
    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);

    // Create a compatible bitmap and select it into the memory device context
    HBITMAP hBitmap = CreateCompatibleBitmap(hScreenDC, screenWidth, screenHeight);
    SelectObject(hMemoryDC, hBitmap);

    // Copy the screen into the bitmap
    BitBlt(hMemoryDC, 0, 0, screenWidth, screenHeight, hScreenDC, 0, 0, SRCCOPY);

    // Save the bitmap as a PNG file using GDI+
    Bitmap bitmap(hBitmap, NULL);
    CLSID pngClsid;
    GetEncoderClsid(L"image/png", &pngClsid);
    bitmap.Save(std::wstring(filename.begin(), filename.end()).c_str(), &pngClsid, NULL);

    // Cleanup
    DeleteObject(hBitmap);
    DeleteDC(hMemoryDC);
    ReleaseDC(NULL, hScreenDC);

    // Shutdown GDI+
    GdiplusShutdown(gdiplusToken);

    cout << "Screenshot saved as " << filename << endl;
}

// Function to get the CLSID of the encoder (needed for saving PNG)
int GetEncoderClsid(const WCHAR* format, CLSID* pClsid) {
    UINT num = 0; 
    UINT size = 0;

    ImageCodecInfo* pImageCodecInfo = NULL;

    GetImageEncodersSize(&num, &size);
    if (size == 0) return -1;

    pImageCodecInfo = (ImageCodecInfo*)(malloc(size));
    if (pImageCodecInfo == NULL) return -1;

    GetImageEncoders(num, size, pImageCodecInfo);

    for (UINT j = 0; j < num; ++j) {
        if (wcscmp(pImageCodecInfo[j].MimeType, format) == 0) {
            *pClsid = pImageCodecInfo[j].Clsid;
            free(pImageCodecInfo);
            return j;  // Return success
        }
    }

    free(pImageCodecInfo);
    return -1;
}

// Function to send a file to the server
void sendFile(SOCKET sock, const string& filename) {
    ifstream file(filename, ios::binary);
    if (!file) {
        cerr << "Error opening file!" << endl;
        return;
    }

    // Read file contents into a buffer
    string fileData((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());

    // Send the file size first (to inform the server how much data to expect)
    int fileSize = fileData.size();
    send(sock, reinterpret_cast<char*>(&fileSize), sizeof(fileSize), 0);

    // Send the actual file data
    send(sock, fileData.c_str(), fileData.size(), 0);

    file.close();
    cout << "Screenshot sent to server." << endl;
}

void sendActivityData(SOCKET sock) {
    while (true) {
        string activityMessage = "User is active.";  // Example activity message
        send(sock, activityMessage.c_str(), activityMessage.length(), 0);

        // Capture and send the screenshot every 60 seconds
        CaptureScreenshot("screenshot.png");
        sendFile(sock, "screenshot.png");

        // Wait 60 seconds before sending the next activity
        this_thread::sleep_for(chrono::seconds(60));
    }
}

int main() {
    if (!initializeWinsock()) {
        cerr << "Winsock initialization failed!" << endl;
        return 1;
    }

    SOCKET sock = createSocket();
    if (sock == INVALID_SOCKET) {
        WSACleanup();
        return 1;
    }

    sendActivityData(sock);  // Start sending activity data

    closesocket(sock);
    WSACleanup();
    return 0;
}
