void AddToRegistry() {
    HKEY hKey;
    const char* path = "C:\\Path\\To\\Your\\Program.exe";  // ���� � ����� ���������
    if (RegOpenKeyEx(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Run", 0, KEY_WRITE, &hKey) == ERROR_SUCCESS) {
        RegSetValueEx(hKey, "EmployeeMonitoringClient", 0, REG_SZ, (const BYTE*)path, strlen(path) + 1);
        RegCloseKey(hKey);
    }
}

int main() {
    AddToRegistry();
    while (true) {
        GetLastActiveTime();
        std::string data = "Active";
        SendDataToServer(data);
        Sleep(10000);  // �������� ������ ������ 10 ������
    }
    return 0;
}
