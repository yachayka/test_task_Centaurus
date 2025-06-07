# 1. Структура проекта в Visual Studio

### Для проекта мы создадим два файла:

    Клиентская часть (C++) — отслеживает активность пользователя и отправляет данные на сервер.

    Серверная часть (Python) — принимает данные от клиента и выводит их в консоль.

## Структура папок:

EmployeeMonitoringSystem/

```

├── Client/
│   ├── Client.cpp        // Код клиента на C++
│   └── CMakeLists.txt    // Если будет использоваться CMake (опционально)
└── Server/
    ├── server.py         // Код сервера на Python
    └── requirements.txt  // Зависимости (например, Flask)

```

# 2. Код клиентской части (C++)

## Client.cpp

Клиент будет отслеживать активность пользователя и отправлять эту информацию на сервер через сокеты. В коде будет использоваться Windows API для получения времени последнего ввода.

Вот как должен выглядеть файл Client.cpp:
```

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
    } else {
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

```

### Пояснение:

`GetLastInputInfo` — позволяет узнать время последнего ввода пользователя.

`SendDataToServer` — отправляет данные (например, о времени последнего ввода) на сервер через сокет.

`std::this_thread::sleep_for` — делает паузу в 10 секунд между отправками данных.

# 3. Подключение к серверу через сокеты

Настройка библиотеки сокетов:

Важно, что для работы с сокетами в Windows, используется ws2_32.lib. Мы подключаем его через директиву `#pragma comment(lib, "ws2_32.lib")`, чтобы использовать Windows сокеты.

# 4. Код серверной части (Python)

Теперь создадим сервер, который будет слушать входящие соединения на порту 12345 и выводить полученные данные от клиента.

`server.py`

```

import socket

def start_server():
    server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server_socket.bind(("0.0.0.0", 12345))  # Слушаем на порту 12345
    server_socket.listen(5)
    print("Server listening on port 12345...")

    while True:
        client_socket, addr = server_socket.accept()
        print(f"Connection from {addr}")

        data = client_socket.recv(1024)  # Получаем данные от клиента
        print(f"Received data: {data.decode('utf-8')}")  # Выводим данные

        client_socket.close()  # Закрываем соединение

if __name__ == "__main__":
    start_server()

```

Пояснение:

Этот сервер использует сокеты для прослушивания порта 12345.

Когда клиент подключается и отправляет данные, сервер получает их и выводит в консоль.

Сервер постоянно работает в цикле, ожидая новых подключений.

# 5. Зависимости для сервера

Если планируется использовать Flask или другие библиотеки для дальнейших улучшений (например, веб-интерфейс), можно создать файл requirements.txt, в котором будет указан список необходимых библиотек.

`requirements.txt`

```

Flask==2.0.1

```
Если решишь расширить сервер до веб-интерфейса, Flask будет полезен для создания REST API или веб-страниц.

# 6. Тестирование

    Запуск клиента: В Visual Studio собери и запусти клиентскую программу. Клиент будет работать в фоновом режиме, отслеживать активность пользователя и отправлять данные на сервер.

    Запуск сервера: Запусти сервер через Python. Он должен начать слушать на порту 12345.

    Проверка: На сервере ты увидишь, как клиент отправляет данные.
