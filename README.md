> [!NOTE]
> Task:
> 
> Client-Server application for monitoring work activity
> 
> Explanation
> Simple application to show current work activity of all employers in organisation
> 
> Example applications 
> https://www.teramind.co/solutions/employee-monitoring-software , 
> https://veriato.com/product/
> 
> Client (windows) - c/c++
> Silent launches on user logon and work in background
> Communicates with server at any protocol
> You can't use third-party libraries like boost and others, and you can't use frameworks like Qt and others. 
> 
> Server - desktop or web interface - any language 
> List all connected clients - domain/machine/ip/user
> Show client’s last active time
> Ability to get screenshot from client’s desktop 
> 
> In response send link to github.com project page, which contains all Visual Studio solution files with full source code and dependencies if any.


![made_with](https://img.shields.io/badge/Language-С++-purple)
![language](https://img.shields.io/badge/Language-Python-blue)

# Employee Activity Monitoring

## Описание

Простое клиент-серверное приложение для мониторинга текущей активности сотрудников в организации.

- **Клиент (Windows, C/C++)**  
  - Запускается тихо при входе пользователя  
  - Работает в фоне  
  - Общается с сервером по TCP без сторонних библиотек (Boost, Qt и т.п. не используются)  
  - Отправляет данные о пользователе, машине, IP и активности  
  - По запросу сервера делает скриншоты рабочего стола и отправляет их

- **Сервер (Python, Flask)**  
  - Веб-интерфейс для отображения подключённых клиентов  
  - Показывает домен, имя машины, IP, пользователя и время последней активности  
  - Позволяет получить скриншот с клиента



## Структура репозитория

```
test_task_Centaurus/
│
├── client/                     # Client-side application (C/C++)
│   ├── client.cpp              # Main client program
│   ├── screenshot.cpp          # Screen capture functionality
│   ├── activity_monitor.cpp    # Activity monitoring logic (user idle time)
│   ├── client.h                # Header file for client functions
│   ├── Makefile                # Build script (if using Make)
│   └── config.ini              # Configuration file (e.g., server address, heartbeat frequency)
│
├── server/                     # Server-side application (Python Flask server)
│   ├── server.py               # Main server program (Flask)
│   ├── templates/              # HTML templates for the UI
│   │   └── index.html          # Frontend to display connected clients
│   └── requirements.txt        # Dependencies for Python server
│
└── README.md                   # Project overview and setup instructions

```



## Сборка и запуск

### Клиент

1. Откройте `client/Client.sln` в Visual Studio.
2. Соберите проект.
3. Запустите `Client.exe` или добавьте его в автозагрузку Windows.

### Сервер

1. Установите Python 3.8+.
2. Перейдите в папку `server`.
3. Создайте виртуальное окружение и активируйте его:


```
python -m venv venv
venv\Scripts\activate # Windows
```


4. Установите зависимости:

```
pip install -r requirements.txt
```

5. Запустите сервер:

```
python app.py
```


6. Откройте в браузере [http://localhost:5000](http://localhost:5000).



## Использование

- Клиент автоматически отправляет данные о текущей активности.
- Сервер отображает список клиентов и их статус.
- По запросу сервера клиент отправляет скриншот рабочего стола.



## Особенности реализации

- Клиент использует только стандартный Windows API и Winsock2 для сетевого взаимодействия.
- Скриншоты делаются через GDI+.
- Сервер реализован на Flask с простым REST API.
- Нет сторонних библиотек на клиенте, как требовалось в задании.



## Ссылки

- Репозиторий: [https://github.com/yachayka/test_task_Centaurus](https://github.com/yachayka/test_task_Centaurus)

