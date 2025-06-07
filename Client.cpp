#include <windows.h>
#include <winsock2.h>
#include <iostream>
#include <string>
#include <chrono>
#include <thread>

#pragma comment(lib, "ws2_32.lib")

// Функция для получения времени последнего ввода
void GetLastActiveTime() {
    LASTINPUTINFO lii;
    lii.cbSize = sizeof(LASTINPUTINFO);
    if (GetLastInputInfo(&lii)) {
        DWORD dwMilliseconds = GetTickCount() - lii.dwTime;
        std::cout << "Last input was " << dwMilliseconds / 1000 << " seconds ago.\n";
    }
    else {
        std::cerr << "Error getting last input time.\n";
    }
}

// Функция для отправки данных на сервер
void SendDataToServer(const std::string& data) {
    WSADATA wsaData;
    SOCKET sock;
    sockaddr_in server;

    WSAStartup(MAKEWORD(2, 2), &wsaData);
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) {
        std::cerr << "Error creating socket.\n";
        return;
    }

    server.sin_family = AF_INET;
    server.sin_port = htons(12345);  // Порт сервера
    server.sin_addr.s_addr = inet_addr("127.0.0.1");  // IP сервера (localhost)

    if (connect(sock, (struct sockaddr*)&server, sizeof(server)) == SOCKET_ERROR) {
        std::cerr << "Connection failed.\n";
        closesocket(sock);
        return;
    }

    send(sock, data.c_str(), data.length(), 0);
    closesocket(sock);
    WSACleanup();
}

int main() {
    while (true) {
        // Получаем время последнего ввода пользователя
        GetLastActiveTime();

        // Отправляем данные на сервер (например, активность пользователя)
        std::string data = "User is active";  // Можешь дополнить эти данные
        SendDataToServer(data);

        // Ждем 10 секунд перед следующим отправлением
        std::this_thread::sleep_for(std::chrono::seconds(10));
    }
    return 0;
}
