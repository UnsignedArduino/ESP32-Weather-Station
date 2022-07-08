#include <Arduino.h>
#include <SPIFFS.h>

void listFiles();

#ifdef ESP32
void listDir(fs::FS &fs, const char * dirname, uint8_t levels);
#endif
