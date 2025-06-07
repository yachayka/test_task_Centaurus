import socket

def start_server():
    server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server_socket.bind(("0.0.0.0", 12345))  # ������� �� ����� 12345
    server_socket.listen(5)
    print("Server listening on port 12345...")

    while True:
        client_socket, addr = server_socket.accept()
        print(f"Connection from {addr}")

        data = client_socket.recv(1024)  # �������� ������ �� �������
        print(f"Received data: {data.decode('utf-8')}")  # ������� ������

        client_socket.close()  # ��������� ����������

if __name__ == "__main__":
    start_server()
