from flask import Flask, render_template, request
from flask_socketio import SocketIO, send, emit, join_room
from gevent import pywsgi
from geventwebsocket.handler import WebSocketHandler
import socket
import threading
import os
from dotenv import load_dotenv
from cryptography.fernet import Fernet

# Generate a random encryption key
key = b'HPfamgIJB2gGsC1xPuK2y8DFwBsXMwBtdigSjzUvRgw='

# Create a Fernet cipher object with the key
cipher = Fernet(key)

def encrypt_string(plain_text):
    # Encode the plain text string to bytes
    plain_text_bytes = plain_text.encode('utf-8')

    # Encrypt the bytes using the cipher
    encrypted_bytes = cipher.encrypt(plain_text_bytes)

    # Convert the encrypted bytes to a string
    encrypted_string = encrypted_bytes.decode('utf-8')

    return encrypted_string

def decrypt_string(encrypted_string):
    # Encode the encrypted string to bytes
    encrypted_bytes = encrypted_string.encode('utf-8')

    # Decrypt the bytes using the cipher
    decrypted_bytes = cipher.decrypt(encrypted_bytes)

    # Convert the decrypted bytes to a string
    decrypted_string = decrypted_bytes.decode('utf-8')

    return decrypted_string


load_dotenv()

SERVER_PORT = int(os.getenv('SERVER_PORT'))
USE_LOCAL= os.getenv('USE_LOCAL')


SERVER_IP = "192.168.1.20"

if(USE_LOCAL == "true"):
    SERVER_IP = "192.168.1.4" #Replace with Local IP
else:
    SERVER_IP = ""            #Will be open to an NGROK Server

print(SERVER_IP)
print(SERVER_PORT)

app = Flask(__name__)
app.config['SECRET_KEY'] = 'your-secret-key'
socketio = SocketIO(logger=True, engineio_logger=True, ping_timeout=86400, ping_interval=86400)
socketio.init_app(app, cors_allowed_origins="*")
app.debug = True


receivedHexFile = []


idx = 0
ECUID = 0

token = "xV1uEk56s"

car_connection_status = 0
connected_ECU_ID = 0

@app.route('/')
def index():
    print("Home")
    return render_template('index.html')
    

@socketio.on('W_message')
def handle_message(W_message):
    print(W_message)


@socketio.on('join')
def handle_message(room):
    join_room(room)
    print('Client joined room:', room)



#Connecting to Car
@socketio.on('W_connectCar')
def handle_socket(W_connectCar):
    socketio.emit("E_connectCar", f"Connect Request: {W_connectCar}")

    

@socketio.on('E_connectCar')
def handle_socket(E_connectCar):
    print(E_connectCar)
    if(E_connectCar == "Connection Accepted"):
        car_connection_status = 1
        socketio.emit("W_connectCar", "Connection Accepted", room = 'website')
    elif(E_connectCar == "Bad Token"):
        socketio.emit("W_connectCar", "Bad Token", room = 'website')
    else:
        pass


#Disconnecting from car
@socketio.on('W_disconnectCar')
def handle_socket():
    global car_connection_status
    car_connection_status = 0
    socketio.emit("W_disconnectCar", "Disconnection Accepted", room = 'website')
    

    
# Connecting to ECU
@socketio.on("W_connectECU")
def handle_socket(W_connectECU):
    print(W_connectECU)
    global connected_ECU_ID
    connected_ECU_ID = W_connectECU
    socketio.emit("E_connectECU", f"Connect to ECU {W_connectECU}")

@socketio.on("W_disconnectECU")
def handle_socket(W_disconnectECU):
    print(W_disconnectECU)
    global connected_ECU_ID
    connected_ECU_ID = 0
    socketio.emit("W_disconnectECU", "")



@socketio.on("E_connectECU")
def handle_socket(E_connectECU):
    print(E_connectECU)
    global connected_ECU_ID
    if(E_connectECU == "ECU Connected"):
        socketio.emit("W_connectECU", f"Connection Established With ECU {connected_ECU_ID}", room = 'website')
    elif(E_connectECU == "Invalid ECU ID"):
        socketio.emit("W_connectECU", "Invalid ECU ID", room = 'website')
    elif(E_connectECU == "Invalid Connect Request"):
        socketio.emit("W_connectECU", "Invalid Connection Request", room = 'website')
    else:
        pass

#Receiving and decrypting hex fil
@socketio.on('W_receiveHex')
def handle_socket(W_receiveHex):
    global receivedHexFile
    global idx
    idx = 0

    decrypted_string = decrypt_string(W_receiveHex)
    #print("Decrypted string:", decrypted_string)
    receivedHexFile = decrypted_string.split(',')
    
    for i in range(len(receivedHexFile)):
        print(receivedHexFile[i])

    receivedHexFile[len(receivedHexFile) - 2] = receivedHexFile[len(receivedHexFile) - 2].replace('1FF', '1FF#')
    socketio.emit("W_receiveHex", "OK", room = 'website')


#Flashing
@socketio.on('W_Flash')
def handle_socket(W_Flash):
    if(W_Flash == "Start"):
        socketio.emit("E_Flash", "ready ?")


@socketio.on('E_Flash')
def handle_socket(E_Flash):
    print(E_Flash)
    global idx
    if((idx >= len(receivedHexFile)-2) and (E_Flash != 'again')):
        socketio.emit("E_Flash","Done-EOT")
        idx = 0
    elif(E_Flash == "start"):
        socketio.emit("E_Flash",(':'+receivedHexFile[idx]+'*-EOT'))
    elif(E_Flash == "next"):
        idx+=1
        socketio.emit("E_Flash",(':'+receivedHexFile[idx]+'*-EOT'))
        socketio.emit("W_progress", (((idx/len(receivedHexFile)) * 100) * 0.8), room = 'website')
    elif(E_Flash == 'again'):
        socketio.emit("E_Flash",(':'+receivedHexFile[idx]+'*-EOT'))
    elif(E_Flash == "Master Flashed"):
        socketio.emit("W_Flash", "Master Flashed", room = 'website')


@socketio.on('E_ECU_Flash')
def handle_socket(E_ECU_Flash):
    
    if(E_ECU_Flash == f"ECU {connected_ECU_ID} Backing up"):

        socketio.emit("W_Flash", f"ECU {connected_ECU_ID} Backing up", room = 'website')
        socketio.emit("W_progress", "85", room = 'website')

    elif(E_ECU_Flash == f"ECU {connected_ECU_ID} Started Flashing"):

        socketio.emit("W_Flash", f"ECU {connected_ECU_ID} started Flashing", room = 'website')
        socketio.emit("W_progress", "90", room = 'website')

    elif(E_ECU_Flash == "ECU Flashing Error"):
        socketio.emit("W_Flash", "ECU Flashing Error, Restoring from backup", room = 'website')

    elif(E_ECU_Flash == f"ECU {connected_ECU_ID} Flashed"):
        socketio.emit("W_progress", "100", room = 'website')
    else:
        pass
    
@socketio.on('connect')
def handle_connect():
    print("Client connected ")
    socketio.emit("message", 'Server connected')

@socketio.on('disconnect')
def handle_disconnect():
    print('Client disconnected')
    socketio.emit("message", 'Server disconnected')

@socketio.on('heartbeat')
def handle_heartbeat(heartbeat):
    socketio.emit("heartbeat", "alive")


if __name__ == "__main__":
    pywsgi.WSGIServer((SERVER_IP, int(SERVER_PORT)), app, handler_class=WebSocketHandler).serve_forever()
