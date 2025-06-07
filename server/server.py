from flask import Flask, render_template, request
import socket

app = Flask(__name__)

# Sample in-memory data (you can replace this with a database for real applications)
clients = [
    {'ip': '192.168.0.101', 'user': 'JohnDoe', 'last_active': '2025-06-07 10:15:00'},
    {'ip': '192.168.0.102', 'user': 'JaneSmith', 'last_active': '2025-06-07 09:50:00'}
]

@app.route('/')
def index():
    return render_template('index.html', clients=clients)

@app.route('/screenshot/<ip>')
def screenshot(ip):
    # This endpoint can be used to trigger screenshot requests to the client.
    # In a real implementation, you'd send a signal to the client to capture and send the screenshot.
    return f"Requesting screenshot from {ip}"

if __name__ == '__main__':
    app.run(debug=True, host='0.0.0.0', port=5000)  # Expose on localhost
