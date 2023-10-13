# Python WebSocket Server

## Guide to Develop Locally

### Downloading the Project

- Install **VSCode** ([Download](https://code.visualstudio.com/download]))
- Install **Python** ([Download](https://www.python.org/downloads/))
- Install **Python Extension for VSCode** ([Download](https://marketplace.visualstudio.com/items?itemName=ms-python.python))
- Download the source code by clicking the green button with the code icon and then clicking 'Download ZIP'
  ![Download_Screenshot](https://github.com/MuhammadM1998/iti-mcu/assets/86230182/afa819db-51c4-4e18-9873-bb29afd9247a)
- Unzip the file and open the folder in VSCode

### Installing Dependencies

- Open VSCode Terminal or your OS Terminal in the project folder.
- Install `pipreqs` which scans `.py` files and generates a file with all the needed dependencies automatically.
  ```bash
  pip install pipreqs
  ```
- Run `pipreqs`. A `requirements.txt` file should be generated after it's done.
  ```bash
  pipreqs
  ```
- Install the dependencies generated in `requirements.txt`
  ```bash
  pip install -r requirements.txt
  ```
- Install the dependencies generated in `requirements.txt`
  ```bash
  pip install -r requirements.txt
  ```
- Run the server
  ```bash
  python server.py
  ```
- You should now be able to call the server by calling `localhost:9999`.
