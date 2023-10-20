from flask import Flask
from flask_socketio import SocketIO, send, emit
from gevent import pywsgi
from geventwebsocket.handler import WebSocketHandler
import os
from dotenv import load_dotenv

load_dotenv()

# Server IP and Port
USE_LOCAL = os.getenv("USE_LOCAL")
SERVER_PORT = int(os.getenv("SERVER_PORT"))
SERVER_IP = "192.168.1.20"

if USE_LOCAL == "true":
    SERVER_IP = "192.168.1.20"
else:
    SERVER_IP = ""

print('Server started. Listening on', SERVER_IP, ':', SERVER_PORT)

# Flask and SocketIO Setup
app = Flask(__name__)
app.config["SECRET_KEY"] = "your-secret-key"
socketio = SocketIO(app)
socketio = SocketIO(logger=True, engineio_logger=True, ping_timeout=86400, ping_interval=86400)
socketio.init_app(app, cors_allowed_origins="*")
app.debug = True


# Connecting to server
@socketio.on("connect")
def handle_connect():
    print("Client connected ")
    socketio.emit("message", "Server connected successfully")


# Disconnecting from server
@socketio.on("disconnect")
def handle_disconnect():
    print("Client disconnected")
    socketio.emit("message", "Server disconnected successfully")


# Receive message from client
@socketio.on("message")
def handle_message(message):
    print(message)
    socketio.emit("message", "Call Back Working With Update")


# Connecting to car
@socketio.on("connect_car")
def handle_socket():
    print("Client requested connecting To Car...")
    # A placeholder for now. Should check if Car ESP is connected or not first
    socketio.emit("connect_car", "true")

# Connecting to ecu
@socketio.on("connect_ecu")
def handle_socket(ecu_number):
    print("Client requested connecting to ECU...")
    # A placeholder for now. Should check if ECU is connected or not first
    socketio.emit("connect_ecu", "true")

# Receiving hex file
@socketio.on("receive_hex")
def handle_socket(hex_file):
    print("Client sent a hex file...")
    # A placeholder for now. Should check if hex file is valid or not first
    socketio.emit("connect_ecu", "true")

# Opening hex file
hex_list = []
hex_file = open("Bootloader_test.hex", "r")
hex_list = hex_file.read().splitlines()

# Flashing hex file to MCU
file_index = 0
@socketio.on("Flash")
def handle_socket(Flash):
    global file_index
    if (file_index == len(hex_list) - 1) and (Flash != "again"):
        socketio.emit("Flash", "Done-EOT")
    elif Flash == "start":
        socketio.emit("Flash", (":" + hex_list[file_index] + "-EOT"))
    elif Flash == "next":
        file_index += 1
        socketio.emit("Flash", (":" + hex_list[file_index] + "-EOT"))
    elif Flash == "again":
        socketio.emit("Flash", (":" + hex_list[file_index] + "-EOT"))


if __name__ == "__main__":
    pywsgi.WSGIServer((SERVER_IP, int(SERVER_PORT)), app, handler_class=WebSocketHandler).serve_forever()
