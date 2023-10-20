from flask import Flask
from flask_socketio import SocketIO
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
    # Echo back message. Client checks if message is echoed back to let the user know what's happening
    socketio.emit("message", {"state": "true", "message": message})


# Connecting to car
@socketio.on("connect_car")
def handle_socket():
    print("Client requested connecting To Car...")
    socketio.emit("connect_car", "true") # Temporarily always return true for now

# Connecting to ecu
@socketio.on("connect_ecu")
def handle_socket(ecu_number):
    print("Client requested connecting to ECU Number:", ecu_number)
    socketio.emit("connect_ecu", "true") # Temporarily always return true for now

# Receiving hex file
@socketio.on("receive_hex")
def handle_socket(hex_file):
    print("Client sent a hex file...")
    socketio.emit("connect_ecu", "true") # Temporarily always return true for now

# Opening hex file
# الجزء ده مفروض يبقي ف الايفنت اللي فوق
hex_list = []
hex_file = open("Bootloader_test.hex", "r")


# Verifying hex file
@socketio.on("verify_hex")
# هنا المفروض ما يبقاش في بارامتر لانك هتستخدم الفايل اللي اتبعت ف الايفنت اللي فوق
def handle_socket():
    print("Client requests verifying the hex file...")
    # المفروض تتحقق من الملف صالح ولا لأ وترجع true او false
    socketio.emit("verify_hex", "true")

# Flashing hex file to MCU
file_index = 0
@socketio.on("flash_hex")
def handle_socket(Flash):
    # الحتة بتاعة تقسيم الملف لسطور خليها هنا جوه الايفنت بتاع الفلاش عشان هي بتستخدم هناك مش اول ما تستقبل الملف
    # انت بتقسم الفايل لسطور عشان تبعت للمايكرو كنترولر سطر ورا التاني ف كده معاك عدد السطور
    # المطلوب انك بعد كل سطر تبعته للمايكرو كنترولر وتتأكد انه اتبعت بتبعت ايفنت للكلاينت انه السطر ده خلص
    # عشان عندي هشوف عدد السطور ف الفايل بردو وكل ما انت تبعتلي رقم سطر جديد اتبعت هحدث ال
    # progress bar
    hex_list = hex_file.read().splitlines() 

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
