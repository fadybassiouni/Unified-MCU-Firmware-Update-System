from flask import Flask, render_template, request
from flask_socketio import SocketIO, send, emit
from gevent import pywsgi
from geventwebsocket.handler import WebSocketHandler
import socket
import threading


app = Flask(__name__)
app.config['SECRET_KEY'] = 'your-secret-key'
socketio = SocketIO(app)
socketio = SocketIO(logger=True, engineio_logger=True)
socketio.init_app(app, cors_allowed_origins="*")

#Open Hex File and Split it into lines
hex_list = []
hex_file = open('C:\\Users\\fady3\\Desktop\\AVR\\Bootloader_test.hex', 'r')
hex_list = hex_file.read().splitlines()

# Create a socket
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

# Bind the socket to a specific address and port
sock.bind(('localhost', 9999))
# Listen for incoming connections
sock.listen()

# Handling Messages From ESP
def handle(client):
    while True:
        response = client.recv(1024).decode('ascii')

        if(response == "start"):

            i=0
            client.send((hex_list[i]+'-EOT\n').encode('ascii'))
            response = client.recv(1024).decode('ascii')
            print("Response: "+response)

            while i<len(hex_list):

                if(response == 'next'):
                    i+=1
                    if i == len(hex_list):
                        break;
                    client.send((hex_list[i]+'-EOT\n').encode('ascii'))
                
                elif(response == 'again'):
                    client.send((hex_list[i]+'-EOT\n').encode('ascii'))
                    
                response = client.recv(1024).decode('ascii')
                print("Response: "+response)

        client.send("Done-EOT\n".encode('ascii'))
        break;

def ESPhandle(connection, address):
    while True:
        # Receive data from the socket
        data = connection.recv(1024)
        
        # Send a response back to the client
        response = 'Received From: ' + data.decode('utf-8')
        print(f"Received Data From {address}: {data}")
        connection.sendall(response.encode('utf-8'))

@app.route('/')
def index():
    print("Connected")
    return render_template('index.html')

@socketio.on('message')
def handle_message(message):
    print(message.get("data1"))
    print(message.get("data2"))
    print(message.get("data3"))
    socketio.emit("myevent", "EVENT DATA HERE...")

@socketio.on('mo7sen')
def handle_message(mo7sen):
    print(mo7sen)
    socketio.emit("myevent", "EVENT DATA HERE...")

@socketio.on('Connect MCU')
def handle_socket():
    print("Connecting To MCU...")
    # Accept a connection
    conn, addr = sock.accept()

    print(f"Connected with {addr}")
    
    thread = threading.Thread(target=ESPhandle, args=(conn,addr))
    thread.start() 
    socketio.emit("MCU Connected", "Connected")

@socketio.on('connect')
def handle_connect():
    print("Client connected ")

@socketio.on('disconnect')
def handle_disconnect():
    print('Client disconnected')

#if __name__ == '__main__':
#    socketio.run(app, host='0.0.0.0', port=55555)

if __name__ == "__main__":
    
    pywsgi.WSGIServer(("", 55555), app, handler_class=WebSocketHandler).serve_forever()

#,keyfile='key.pem', certfile='cert.pem'-------,keyfile='key.pem', certfile='cert.pem'
