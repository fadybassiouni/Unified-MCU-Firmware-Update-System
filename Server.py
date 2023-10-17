from flask import Flask, render_template, request
from flask_socketio import SocketIO, send, emit
from gevent import pywsgi
from geventwebsocket.handler import WebSocketHandler
import socket
import threading
import os
from dotenv import load_dotenv

load_dotenv()

SERVER_PORT = int(os.getenv('SERVER_PORT'))
USE_LOCAL= os.getenv('USE_LOCAL')


SERVER_IP = "192.168.1.20"

if(USE_LOCAL == "true"):
    SERVER_IP = "192.168.1.20"
else:
    SERVER_IP = ""

app = Flask(__name__)
app.config['SECRET_KEY'] = 'your-secret-key'
socketio = SocketIO(app)
socketio = SocketIO(logger=True, engineio_logger=True, ping_timeout=86400, ping_interval=86400)
socketio.init_app(app, cors_allowed_origins="*")
app.debug = True

# sOpen Hex File and Split it into lines
hex_list = []
hex_file = open('./resources/sample.hex', 'r')
hex_list = hex_file.read().splitlines()

# # Create a socket
# sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

# # Bind the socket to a specific address and port
# sock.bind(('localhost', 9999))

# # Listen for incoming connections
# sock.listen()

idx = 0

# sock.settimeout(10)

# ESP_state = "not connected"

# Handling Messages From ESP
# def flash(client):
#     while True:
#         response = client.recv(1024).decode('ascii')

#         if(response == "start"):

#             i=0
#             client.send((hex_list[i]+'-EOT\n').encode('ascii'))
#             response = client.recv(1024).decode('ascii')
#             print("Response: "+response)

#             while i<len(hex_list):

#                 if(response == 'next'):
#                     i+=1
#                     if i == len(hex_list):
#                         break;
#                     client.send((hex_list[i]+'-EOT\n').encode('ascii'))
                
#                 elif(response == 'again'):
#                     client.send((hex_list[i]+'-EOT\n').encode('ascii'))
                    
#                 response = client.recv(1024).decode('ascii')
#                 print("Response: "+response)

#         client.send("Done-EOT\n".encode('ascii'))
#         break;



            

@app.route('/')
def index():

    print("Home")
    return render_template('index.html')
    

@socketio.on('message')
def handle_message(message):
    print(message)
    socketio.emit("echo", message)


@socketio.on('connect_car')
def handle_socket(Connect_MCU):
    print("Connecting To MCU...")
    socketio.emit("connect_car", 'true')  # A placeholder for now. Should check if ESP is connected or not first


@socketio.on('Flash')
def handle_socket(Flash):
    global idx
    if((idx == len(hex_list)-1) and (Flash != 'again')):
        socketio.emit("Flash","Done-EOT")
    elif(Flash == "start"):
        socketio.emit("Flash",(':'+hex_list[idx]+'-EOT'))
    elif(Flash == "next"):
        idx+=1
        socketio.emit("Flash",(':'+hex_list[idx]+'-EOT'))
    elif(Flash == 'again'):
        socketio.emit("Flash",(':'+hex_list[idx]+'-EOT'))



@socketio.on('connect')
def handle_connect():
    print("Client connected ")
    socketio.emit("message", 'Server connected successfully')

@socketio.on('disconnect')
def handle_disconnect():
    print('Client disconnected')
    socketio.emit("message", 'Server disconnected successfully')

#if __name__ == '__main__':
#    socketio.run(app, host='0.0.0.0', port=55555)

if __name__ == "__main__":
    pywsgi.WSGIServer((SERVER_IP, int(SERVER_PORT)), app, handler_class=WebSocketHandler).serve_forever()

















#,keyfile='key.pem', certfile='cert.pem'-------,keyfile='key.pem', certfile='cert.pem'
#GET /socket.io/?EIO=4&transport=websocket HTTP/1.1 52
#Sec-WebSocket-Version: 13 27
#Sec-WebSocket-Key: 1Mrz+ZvS7YP5Sd/RjSdwXQ== 45
#Connection: Upgrade 21
#Upgrade: websocket 20
#Sec-WebSocket-Extensions: permessage-deflate; client_max_window_bits 70
#Host: 4.tcp.eu.ngrok.io:12490 31

#+IPD,217:HTTP/1.1 101 Switching Protocols
#Upgrade: websocket
#Connection: Upgrade
#Sec-WebSocket-Accept: I9+n29HSvMByFX8TCMlGzqq/XtE=

#⸮V0{"sid":"tsjWH1TCDoUndquTAAAA","upgrades":[],"pingTimeout":20000,"pingInterval":25000}
#+IPD,3:⸮2

#40{"sid":"NCQrAG-k2ZPB3SIyAAAG"}
#40{"sid":"iYXdxCCsO03uMrLMAAAN"}
