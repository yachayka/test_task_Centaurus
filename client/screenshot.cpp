#include <windows.h>

void CaptureScreen() {
    HWND hwndDesktop = GetDesktopWindow();
    HDC hdcScreen = GetDC(hwndDesktop);
    HDC hdcMemory = CreateCompatibleDC(hdcScreen);

    RECT rect;
    GetClientRect(hwndDesktop, &rect);
    int width = rect.right;
    int height = rect.bottom;

    HBITMAP hBitmap = CreateCompatibleBitmap(hdcScreen, width, height);
    SelectObject(hdcMemory, hBitmap);
    BitBlt(hdcMemory, 0, 0, width, height, hdcScreen, 0, 0, SRCCOPY);

    // Save the bitmap (for simplicity, let's assume saving is done here)
    // In a real application, you might save to a file and send to the server
    DeleteDC(hdcMemory);
    ReleaseDC(hwndDesktop, hdcScreen);
}
