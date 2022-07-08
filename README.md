# ESP32-Weather-Station

[https://github.com/Bodmer/OpenWeather/tree/master/examples/TFT_eSPI_OpenWeather](https://github.com/Bodmer/OpenWeather/tree/master/examples/TFT_eSPI_OpenWeather), but fixed to work on PlatformIO!

This branch contains the the original TFT_eSPI_OpenWeather example by Bodmer, only modified to just compile and upload on PlatformIO. 

Go to the [`modifications`](https://github.com/UnsignedArduino/ESP32-Weather-Station/tree/modifications) branch to see my changes!

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

** These can be changed in `src/User_Setup.h` file for TFT_eSPI

## Software
1. Clone this repo somewhere.
2. Open in your preferred IDE. (for example, VS Code with PlatformIO plugin)
3. Rename `src/All_Settings template.h` to `src/All_Settings.h` and change your settings.
4. Run `PlatformIO: Project Tasks - <board name> - General - Build`. (So the libraries are downloaded)
5. Copy the contents of `src/User_Setup.h` to `.pio\libdeps\<your board name>\TFT_eSPI\User_Setup.h`.
6. Change `upload_port` and `monitor_port` in `platformio.ini` if needed. (much more reliable then relying on auto-detect)
7. Run `PlatformIO: Project Tasks - <board name> - General - Upload`.
8. Run `PlatformIO: Project Tasks - <board name> - Platform - Build Filesystem Image`.
9. Run `PlatformIO: Project Tasks - <board name> - Platform - Upload Filesystem Image`.
10. Reset board if needed. 
