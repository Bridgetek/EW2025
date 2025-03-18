1. INTRODUCTION
    This project contains example code for the Bedside Patient Monitor demo application.



2. FOLDER Structure
    ├───Project                  
    │   ├───MM2040EV_BT81x_C | MM2040EV platform with Rp2040 and BT817
    │   ├───Msvc_BT81x       | Microsoft Visual Studio C++ 2022 IDE solution file for FT4222, MPSSE and Emulator platform
    ├───Hdr                  | Header files
    ├───Src                  | Source files
    ├───Test                 | Eve specific Assets: bitmap data, flash image, font data etc. 
    ├───Common               | Eve_Hal framework and helper functions

2. SUPPORTED PLATFORM
    * Microsoft Visual C++ platform with FT4222, MPSSE and Emulator
    * MM2040EV
    
    EVE chip: BT817
    LCD size: WXGA(1280x800)

3. SETUP AND RUN
    3.1 CONNECT HARDWARE
        3.1.1 Microsoft Visual C++ platform with FT4222 and MPSSE
            - Connect PC with EVE platform via FT4222 or MPSSE
            - Connect power to EVE platform

        3.1.2 Microsoft Visual C++ platform with Emulator
            - This setup uses window PC only
        
        3.1.3 MM2040EV
            - Connect PC with MM2040EV via USB cable

    3.2 DOWNLOAD ASSETS

        Copy all files in the Test/Flash folder to the SD card.
        
    3.3 BUILD AND RUN
        3.3.1 Microsoft Visual C++ platform with FT4222, MPSSE and Emulator
            - Open project in Project\MM2040EV_BT81x_C with Microsoft Visual C++
            - Build (Ctrl + B)
            - Run (F5)
        
        3.3.2 MM2040EV
            - Install VScode.
            - Install VScode extensions: Cmake, Raspberry Pi Pico extension.
            - Open folder Project\MM2040EV_BT81x_C with VScode
            - Import Project\MM2040EV_BT81x_C:
                + Open Raspberry Pi Pico extension panel, click button "Import Project"
                + Location: Select path to Project\MM2040EV_BT81x_C
                + Pico SDK: v1.5.1
            - Click "Import"

            - Configure settings.json:
                "cmake.configureArgs": [
                    "-DEVE_APPS_PLATFORM=RP2040",
                    "-DEVE_APPS_GRAPHICS=BT817",
                    "-DEVE_APPS_DISPLAY=WXGA"
                ],
                "raspberry-pi-pico.useCmakeTools": true,
                "cmake.generator": "NMake Makefiles",

            - Click "Compile" button on Raspberry Pi Pico extension's panel
            - Connect RP2040 and EVE to PC, use Zadig to install driver "WinUSB" for RP2040 USB port
            - Open USB mode on RP2040 by pressing "BOOTSEL" while powering the Pico board, copy the build/MM2040EV_BT81x_C.uf2 into RP2040 USB folder, the demo should start after that

4. Demo functionalities
        4.1 Date and time setting: Users can access date and time settings by swiping left to right or right to left.

        4.2 Zoom in / out: Tap the zoom button to zoom the graph from level 1 to 8 (pixels per sample).

        4.3 Date and time formatting:

            - Tap on the date to switch the date format (hh-mm-yyyy, hh-mmm-yyyy, hh-month-yyyy).
            - Tap on the time to switch the time format (hh:mm, hh:mm:ss, hh:mm:ss:ms).

5. CONFIGURATION INSTRUCTIONS
    The application uses the macros to configure the platforms: 
	
    Host platform:
        - Window host: EVE_PLATFORM_FT4222, EVE_PLATFORM_MPSSE
        - Emulator   : EVE_PLATFORM_BT8XXEMU
    
    EVE platform: EVE_GRAPHICS_BT817
    
    Please see common\eve_hal\EVE_Config.h. for more macros.
            
                                   【END】
