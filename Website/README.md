# Nuxt 3 Minimal Starter

Look at the [Nuxt 3 documentation](https://nuxt.com/docs/getting-started/introduction) to learn more.

## Roadmap

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

## Setup Locally on your PC

- Install **VSCode** ([Download](https://code.visualstudio.com/download]))
- Install **Node** ([Download](https://nodejs.org/dist/v18.18.0/node-v18.18.0-x64.msi))
- Install **pnpm**. Open the terminal and running this command
  ```bash
  npm install -g pnpm
  ```
- Download the source code by clicking the green button with the code icon and then clicking 'Download ZIP'
  ![Download_Screenshot](https://github.com/MuhammadM1998/iti-mcu/assets/86230182/afa819db-51c4-4e18-9873-bb29afd9247a)
- Unzip the file and open the folder in VSCode
- Create a `.env` file that matches the contents of `.env.example`. Change `SOCKET_HOST` value to the server url if needed.
- Install the dependencies by running the following command in the terminal
  ```bash
  pnpm install
  ```
- Run the development server by running the following command in the terminal
  ```bash
  pnpm run dev
  ```
- You should now be able to visit the site by opening `http://localhost:3000` in your browser 🚀
