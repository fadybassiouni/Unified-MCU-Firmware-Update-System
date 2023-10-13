from flask import Flask, render_template, request
from flask_socketio import SocketIO, send, emit
from gevent import pywsgi
from geventwebsocket.handler import WebSocketHandler
import socket
import threading
import os
from dotenv import load_dotenv

load_dotenv()

SERVER_PORT = os.getenv('SERVER_PORT')
USE_LOCAL= os.getenv('USE_LOCAL')


SERVER_IP = "192.168.1.11"

if(USE_LOCAL == "true"):
    SERVER_IP = "192.168.1.11"
else:
    SERVER_IP = ""

app = Flask(__name__)
app.config['SECRET_KEY'] = 'your-secret-key'
socketio = SocketIO(app)
socketio = SocketIO(logger=True, engineio_logger=True, ping_timeout=86400, ping_interval=86400)
socketio.init_app(app, cors_allowed_origins="*")
app.debug = True

#Open Hex File and Split it into lines
hex_list = []
hex_file = open('C:\\Users\\fady3\\Desktop\\AVR\\Bootloader_test.hex', 'r')
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


@app.route('/mcu')
def mcu():
    print("Connecting To MCU...")
    # Accept a connection
    global conn
    global addr
    global sock
    try:
        conn, addr = sock.accept()

        print(f"Connected with {addr}")

        global ESP_state
        ESP_state = "connected"

        socketio.emit("MCU Connected", "Connected")
        print("Connected")
        return "Connected"
    except socket.timeout:
        print("Timed Out")
        return "Timeout"

    

@socketio.on('message')
def handle_message(message):
    print(message)
    
    socketio.emit("Echo", message)


@socketio.on('Connect MCU')
def handle_socket(Connect_MCU):
    print("Connecting To MCU...")
    global ESP_state
    global conn

    if(ESP_state == "connected"):
        try:
            conn.send("Connect Request-EOT".encode())
            response = conn.recv(1024).decode("ascii")
            print(response)
            if(response == "Accepted"):
                socketio.emit("MCU Connected", "Connected")
                print("Connected to MCU")
                ESP_state = "ready"
            else:
                socketio.emit("MCU Connected", "Failed to Connect")
                print("Failed to Connect to MCU")
        except socket.timeout:
            socketio.emit("MCU Connected", "Timeout")
            print("Timeout")

    else:
        print("ESP Socket Not Connected")

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

@socketio.on('disconnect')
def handle_disconnect():
    print('Client disconnected')

#if __name__ == '__main__':
#    socketio.run(app, host='0.0.0.0', port=55555)

if __name__ == "__main__":
    
    pywsgi.WSGIServer((SERVER_IP, SERVER_PORT), app, handler_class=WebSocketHandler).serve_forever()

















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