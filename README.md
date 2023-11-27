# UMFUS

## Hardware Requirments
 - 4x STM32F103c8T6 MCUs (Bluepill Board)
 - 4x CAN Transceivers (preferably 3.3v ones)
 - ST-Link Debuggers for Uploading the code onto the MCUs
 - 1x ESP8266 WIFI Module (Preferably Flashed with the uploaded Firmware on this Repo)
 - 2x LEDs
 - 1x Ultrasonic Sensor
 - 1x Buzzer
 - 1x DHT11 Sensor
 - 1x L283D Motor Driver
 - 3x AT24C256 EEPROM
 - 1x USB-TTL Serial Converter (Optional)

## Downloading the Project


- Download the source code by clicking the green button with the code icon and then clicking 'Download ZIP' or by Cloning the Project 
     ```bash
    git clone https://github.com/fadybassiouni/UMFUS.git
     ```
- Unzip the file and open the folder
- Import the Source and header files in your preferred IDE (Eclipse, Keil, etc..) for Each MCU Project individually
- For the Server Code use your preferred Text Editor (Sublime Text, VS Code, etc...)
- For the website create follow the upcoming steps

## Installing Dependencies

- Open VSCode Terminal or your OS Terminal in the project folder.
- Install `pipreqs` which scans `.py` files and generates a file with all the needed dependencies automatically.
  ```bash
  pip install pipreqs
  ```
- Run `pipreqs`. A `requirements.txt` file should be generated after it's done.
  ```bash
  pipreqs --force
  ```
- Install the dependencies generated in `requirements.txt`
  ```bash
  pip install -r requirements.txt
  ```
- Install the dependencies generated in `requirements.txt`
  ```bash
  pip install -r requirements.txt
  ```
  
## Running the Python Socket IO Server

### For running the Server Locally
- Create a `.env` files that matches `.env.example` format & fill in the values.
- Open the Server.py file in your text editor and edit the ip with the local IP of your machine
  ```python
  SERVER_IP = "192.168.1.8" #Replace with Local IP
  ```
- Run the server
  ```bash
  python -m Server
  ```
- You should now be able to call the server by calling `<LOCAL IP>:<PORTNO in env file>`.

### Sharing the Server on the Internet

- After Following the previous steps for running the server locally only you can access the server on your local machine. To enable anyone from anywhere to access it. We'll use ngrok.
- Install **Ngrok** ([Download](https://ngrok.com/download))
- In your terminal, run the following command and replace `SERVER_PORT` with your specified server port.
  ```bash
    ngrok tcp SERVER_PORT
  ```
- A URL should appear which you can share to anyone you want to access the server 🚀
## Running the Website

### Nuxt 3 Minimal Starter
Look at the [Nuxt 3 documentation](https://nuxt.com/docs/getting-started/introduction) to learn more.

### Roadmap

- [x] Connect to server
- [x] Disconnect from server
- [x] Connect to car with token (including Token validation)
- [x] Disconnect from car
- [x] Connection to car timeout
- [x] Connect to ECU
- [x] Connection to ECU timeout
- [x] Disconnect from ECU
- [x] Upload Hex File
- [x] Verify Hex File
- [x] Flash Hex File
- [x] Update Progress Dynamically while Flashing

### Setup Locally on your PC

- Install **Node** ([Download](https://nodejs.org/dist/v18.18.0/node-v18.18.0-x64.msi))
- Install **pnpm**. Open the terminal and running this command
  ```bash
  npm install -g pnpm
  ```
- Create a `.env` file that matches the contents of `.env.example`. Change `SOCKET_HOST` value to the server url if needed.
- Install the dependencies by running the following command in the terminal
  ```bash
  pnpm install
  ```
- Run the development server by running the following command in the terminal
  ```bash
  pnpm run dev
  ```
- You should now be able to visit the site by opening `http://localhost:<PORT>` in your browser 🚀

## Running the code on the MCUs

### FOTA Gateway ECU Configuration

 - Open the `main.c` File and edit the `SSID`, `PASSWORD`, `SERVER`, and `PORT`
      ```C
      #define SSID "Paradox"
      #define PASSWORD "tryagainlaterin2020"
    
      /*Define if to use Local network or a remote NGROK Server*/
      #define USE_LOCAL 1
    
      #if (USE_LOCAL == 1)
    
      #define SERVER "192.168.1.4" /*Server IP Here*/
      #define PORT   55555         /*Server Port Here*/
    
      #else
    
      #define SERVER "4.tcp.eu.ngrok.io"
      #define PORT   17888
    
      #endif
      ```
  - Open the `LSTD_types.h` File and choose if you want to enable UART Debugging or not
       ```C
       /*Define if to use UART Debugging or not*/
        #define UART_DEBUG
       ```
   - Compile the Project and upload onto the Microcontroller using ST-Link Utility or Debug using Open OCD
   - If you choose to enable UART Debug you can open any Serial Monitor to see the output
   - Now the Gateway ECU is always in a state of trying to connect to WIFI and the Socket IO Server

### CAN Bootloader on Application ECUs
   - Open the `main.c` file and edit the CAN ID for each APP ECU
        ```C
        CAN_FilterInit.FilterIdHigh = Std_id_High(0x65D);
    	CAN_FilterInit.FilterIdLow = 0x0000;
    	CAN_FilterInit.FilterMaskIdHigh = Std_id_High(0x65D);
    	CAN_FilterInit.FilterMaskIdLow = 0x0000;
    	```
    - Compile the Project and upload onto the Microcontroller using ST-Link Utility or Debug using Open OCD
    - Now The APP ECU is ready to receive a Hex File to be Flashed using the CAN Bootloader (might need a hardreset in case that the EEPROM isn't preloaded with the Blink APP for the first time only)

### Applications
  - Compile any Application from the repo using your preferred IDE and export a Hex File to be sent to the APP MCU
  - If you want to Flash another app you must follow certain guidelines within your app
    - Make sure your app has EXTI enabled on a GPIO Pin
    - Set the EXTI Mode to Rising Edge
    - Enable NVIC for the EXTI
    - Declare a Bootloader Jump Flag Variable and inside the EXTI IRQ change the flag state
    - Add an if statement inside the Main function to check for the flag
    - when Jumping to the bootloader make sure that the jump address is `0x08000004` and the `SCB_VTOR` is `0x08000000`
