
This folder contains the source code for photo booth demo, shown on EW2024 North America.

This demo is based on Adafruit ESP32-S3 feather board and BT820 demo board.

The demo sheet is at:

https://brtchip.com/ew2024-demo-sheet-on-photobooth/

### Build and run

#### Install tool:
 - vscode and ESP-IDF extension
  
#### Configure the ESP-IDF extension:
 - Open the vscode, configure the ESP-IDF by command: "ESP-IDF: Configure ESP-IDF Extension"
 - The ESP-IDF setup tab will appears, add this setting:
	  - Select ESP-IDF version: -> v5.3.1 or any other version
      - Enter ESP-IDF container directory: -> [USER]\esp
      - Enter ESP-IDF Tools directory (IDF_TOOLS_PATH): ->  -> [USER]\esptools

 - Click "Configure tool" button
 - Wait for the installer finish

#### Compile the source
 - Open Terminal tab in VScode, type:
 
        $ cmd
        $ cd [USER]\esp\[ESP version]\esp-idf
    	$ install.bat
    	$ export.bat
    	$ idf.py set-target esp32s3
    	$ idf.py reconfigure
    	$ idf.py menuconfig
    	$ idf.py build

#### Hardware connections

For different chip and host used, the connections may be different. Here show a example diagram of hardware connection, you can change the hardware relatively.

| EVE  | ESP32 |
| ---- | ----- |
| Host | SPI1  |
| VCC  | 3.3V  |
| GND  | GND   |
| DO   | 7     |
| DI   | 8     |
| SK   | 6     |
| CS   | 13    |

#### Run the demo
 - Prepare an SDcard and copy folder "assets_sdcard_eve" into it
 - Insert the SDcard into EVE module

 - On VScode, set the ESP32S3 chip type, by command: "ESP-IDF: Set Espressif device target"
 - Set the COM port, by command: "ESP-IDF: Select Port to Use (COM, tty, usbserial)"
 - Click Burn button or command: "ESP-IDF: Build, Flash and Start a Monitor on your Device"

Demo should start after that

Reference: https://github.com/espressif/vscode-esp-idf-extension/blob/master/docs/tutorial/toc.md

### Build and run with dev container
Dev Containers help streamline the process of setting up your development environment by running your code inside a Docker container, ensuring that all developers use the same setup regardless of their local machine's operating system or configurations

#### Install tool:
 - vscode with ESP-IDF and dev container extension
 - usbipd https://github.com/dorssel/usbipd-win (install it with command: winget install usbipd)

#### Configure the ESP-IDF extension:

On window terminal, share the ESP's locally connected USB devices to other machines (VScode) by:

    usbipd list | grep JTAG | cut -d" " -f1

Assume the above command output "2-3":

    usbipd attach --wsl --busid 2-3
    usbipd bind --busid 2-3

One line version (need wsl or cygwin installed):

    FOR /F "delims=" %i IN ('usbipd list ^| grep JTAG ^| cut -d" " -f1') DO echo %i & usbipd attach --wsl --busid %i & usbipd bind --busid %i

#### Configure the ESP-IDF extension:
Open VScode, open local folder with dev container: "Dev Containers: Reopen Folder Locally"

#### Compile the source
 - Open Terminal tab in VScode, type:
 
        $ cd esp\[ESP version]\esp-idf
    	$ ./install.sh
    	$ ./export.sh
    	$ idf.py set-target esp32s3
    	$ idf.py reconfigure
    	$ idf.py menuconfig
    	$ idf.py build

Run the demo follow guideline:
 - [Hardware connection](#hardware-connections)
 - [Run the demo](#run-the-demo)

Reference: https://github.com/espressif/vscode-esp-idf-extension/blob/master/docs/tutorial/using-docker-container.md

