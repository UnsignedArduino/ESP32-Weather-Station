# ESP32-Weather-Station

[https://github.com/Bodmer/OpenWeather/tree/master/examples/TFT_eSPI_OpenWeather](https://github.com/Bodmer/OpenWeather/tree/master/examples/TFT_eSPI_OpenWeather), but fixed to work on PlatformIO!

This branch contains the modifications I've made for my liking, and requires additional hardware (only for the touch pins) and an extra API call per weather update. (total of 2 calls per update)

Go to the [`main`](https://github.com/UnsignedArduino/ESP32-Weather-Station/tree/main) branch to see Bodmer's TFT_eSPI_OpenWeather example modified only to compile on PlatformIO. 

I did not use the resistive touchscreen on my TFT, as it was too difficult for me (PRs accepted though!) to get it to work. (Mostly calibration problems)

## Hardware

### Parts
You need:
- ESP32
- 320x240 TFT (I used [this one by Adafruit](https://www.adafruit.com/product/1770))
- Some way to wire it all together, probably a breadboard and wires

### Assemble
- ESP32 ground to TFT ground
- ESP32 USB 5v to TFT VIN
- ESP32 18 (VSPI CLK) to TFT CLK
- ESP32 19 (VSPI MISO) to TFT MISO (optional)
- ESP32 23 (VSPI MOSI) to TFT MOSI
- ESP32 5 (VSPI default CS) to TFT CS**
- ESP32 17 to TFT D/C**
- ESP32 32 (T9) to left "button" (touch pin)***
- ESP32 33 (T8) to right "button" (touch pin)***

** These can be changed in `include/User_Setup.h` for TFT_eSPI.
*** These can be changed in `include/All_Settings.h`.

## Software
1. Clone this repo somewhere.
2. Open in your preferred IDE. (for example, VS Code with PlatformIO plugin)
3. Rename `include/All_Settings template.h` to `include/All_Settings.h` and change your settings.
4. Run `PlatformIO: Project Tasks - <board name> - General - Build`. (So the libraries are downloaded)
5. Copy the contents of `include/User_Setup.h` to `.pio\libdeps\<your board name>\TFT_eSPI\User_Setup.h`.
6. Change `upload_port` and `monitor_port` in `platformio.ini` if needed. (much more reliable then relying on auto-detect)
7. Run `PlatformIO: Project Tasks - <board name> - General - Upload`.
8. Run `PlatformIO: Project Tasks - <board name> - Platform - Build Filesystem Image`.
9. Run `PlatformIO: Project Tasks - <board name> - Platform - Upload Filesystem Image`.
10. Reset board if needed. 
