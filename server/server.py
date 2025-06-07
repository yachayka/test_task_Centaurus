import socket
import threading
from datetime import datetime
from flask import Flask, render_template
import os

app = Flask(__name__)

# Global dictionary to store the connected clients' activities and screenshots
clients = {}

SERVER_IP = '0.0.0.0'  # Listen on all available interfaces
SERVER_PORT = 8080

# Handle each client connection
def handle_client(client_socket, client_address):
    global clients
    print(f"New connection from {client_address}")

    try:
        while True:
            # Receive activity message from client
            message = client_socket.recv(1024).decode('utf-8')
            if not message:
                break

            # Update the activity and timestamp for the client in the clients dictionary
            clients[client_address] = {
                'activity': message,
                'last_active': datetime.now().strftime('%Y-%m-%d %H:%M:%S'),
                'screenshot': None  # Placeholder for screenshot data
            }

            # Debugging print to check if the clients dictionary is being updated correctly
            print(f"Received: {message} from {client_address}")
            print(f"Updated clients dictionary: {clients}")

            # Receive the screenshot data (file size + content)
            file_size_data = client_socket.recv(4)  # Read 4 bytes for file size
            if len(file_size_data) < 4:
                break
            file_size = int.from_bytes(file_size_data, byteorder='big')

            file_data = b""
            while len(file_data) < file_size:
                chunk = client_socket.recv(file_size - len(file_data))
                if not chunk:
                    break
                file_data += chunk

            # Save the screenshot to a file
            with open(f"screenshot_{client_address[0]}.png", "wb") as f:
                f.write(file_data)

            # Store the screenshot filename in the clients dictionary
            clients[client_address]['screenshot'] = f"screenshot_{client_address[0]}.png"

    except Exception as e:
        print(f"Error: {e}")
    finally:
        client_socket.close()

# Start the server and listen for incoming connections
def start_server():
    server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server_socket.bind((SERVER_IP, SERVER_PORT))
    server_socket.listen(5)
    print(f"Server listening on {SERVER_IP}:{SERVER_PORT}")

    while True:
        # Accept new client connections
        client_socket, client_address = server_socket.accept()
        client_handler = threading.Thread(target=handle_client, args=(client_socket, client_address))
        client_handler.start()

# Flask route to show the connected clients' activity and screenshot
@app.route('/')
def index():
    return render_template('index.html', clients=clients)

if __name__ == "__main__":
    # Start server thread to handle connections
    server_thread = threading.Thread(target=start_server)
    server_thread.daemon = True
    server_thread.start()

    # Run the Flask web server
    app.run(host='0.0.0.0', port=5000, debug=True)
