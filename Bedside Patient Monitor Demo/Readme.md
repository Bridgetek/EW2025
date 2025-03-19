# Bedside Patient Monitor Demo Application

![image](https://github.com/user-attachments/assets/5467df6e-da1c-4e97-aef2-3834d56d19f0)

## Folder Structure

    ├───Project
    │   ├───MM2040EV_BT81x_C | MM2040EV platform with RP2040 and BT817
    ├───Hdr                  | Header files
    ├───Src                  | Source files
    ├───Test                 | Eve specific Assets: bitmap data, flash image, font data etc.
    └───Common               | Eve_Hal framework and helper functions

## Supported Platforms

 * MM2040EV

		EVE chip: BT817

		LCD size: WXGA (1280x800)

## Setup and Run

### Connect Hardware

 Connect PC with MM2040EV via USB cable

### Download Assets

 Copy all files in the `Test/Flash` folder to an SD card and plug it into MM2040EV.

### Build and Run

#### For MM2040EV:

##### Prepare environment

   * Install Microsoft Visual Studio Community C++ (to get nmake compiler)
   * Install VScode
   * Install VScode extensions: Cmake, Raspberry Pi Pico extension
        
##### Build steps

   * Open "Developer command prompt for VS"
   * In the terminal window, cd to `Project/MM2040EV_BT81x_C`
   * Start VScode by command: `code .`
   * Under VScode, import `Project/MM2040EV_BT81x_C`:
       + Open the Command Palette (Ctrl+Shift+P) and run Raspberry Pi Pico: Import Pico Project
       + Location: Select path to `Project/MM2040EV_BT81x_C`
       + Pico SDK: v1.5.1
       + Cmake version: Select a version (such as 3.31.5)
       + Show Advanced Options -> tick "Enable Cmake-Tools extension integration"
   * Click "Import"
   * After imported, it generates folder `.vscode` and the `.vscode/settings.json`
   * Configure `settings.json`:

			"cmake.configureArgs": [

			"-DEVE_APPS_PLATFORM=RP2040",

			"-DEVE_APPS_GRAPHICS=BT817",

			"-DEVE_APPS_DISPLAY=WXGA"

			],

			"raspberry-pi-pico.useCmakeTools": true,

			"cmake.generator": "NMake Makefiles",
    
   * Open the Command Palette (Ctrl+Shift+P) and run Developer: Reload Window
   * Open the Command Palette (Ctrl+Shift+P) and run "CMake: Build"        
   * A new binary file "build/MM2040EV_BT81x_C.uf2" will be generated
   * Connect RP2040 and EVE to PC. If needed, use Zadig to install driver "WinUSB" for the RP2040's USB port
   * Open USB mode on RP2040 by pressing "BOOTSEL" while powering the Pico board
   * Copy the build/MM2040EV_BT81x_C.uf2 into RP2040's USB folder
   * The demo should start after that
   * At initialization, the demo will prompt you to program the flash with the file from the SD card. Select 'Yes' to proceed. On the next restart, the user should not select 'Yes' again.
     
![image](https://github.com/user-attachments/assets/7f306b64-22b3-444e-89ce-7adbfcbc30e5)

## Demo Functionalities

### Date and time setting

   Users can access date and time settings by swiping left to right or right to left.

### Zoom in / out

   Tap the zoom button to zoom the graph from level 1 to 8 (pixels per sample).

### Date and time formatting

   - Tap on the date to switch the date format (hh-mm-yyyy, hh-mmm-yyyy, hh-month-yyyy).
   - Tap on the time to switch the time format (hh:mm, hh:mm:ss, hh:mm:ss:ms).

## Configuration Instructions

   The application uses the following macros to configure the platforms:

    - EVE_APPS_PLATFORM=RP2040
    - EVE_APPS_GRAPHICS=BT817
    - EVE_APPS_DISPLAY=WXGA

   Please see `common\eve_hal\EVE_Config.h` for more macros.

