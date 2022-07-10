#include <Arduino.h>
#include <SPI.h>
#include <TFT_eSPI.h>
#include "GfxUi.h"
#include "weather_displays.h"
#include "ESP32Touch.h"
#include "SPIFFS_Support.h"
#include <WiFi.h>
#include <JSON_Decoder.h>
#include <OpenWeather.h>
#include "NTP_Time.h"
#include "MoonPhase.h"
#include "All_Settings.h"

TFT_eSPI tft = TFT_eSPI();
GfxUi ui = GfxUi(&tft);

OW_Weather ow;

OW_current* current;
OW_extra* extra;
OW_hourly* hourly;
OW_daily* daily;

bool booting = true;
bool redraw = true;

#define MAX_CAROUSEL_INDEX 3
byte carouselIndex = 0;
String title;

String carouselTitles[MAX_CAROUSEL_INDEX + 1] = {"Weather now", "Hourly forecast", "Daily forecast", "Miscellaneous"};

long lastDownloadUpdate = millis();

ESP32TouchPin leftPin = ESP32TouchPin();
ESP32TouchPin rightPin = ESP32TouchPin();

bool updateData();
void printWeather();

void setup() {
  Serial.begin(9600);

  pinMode(LED_BUILTIN, OUTPUT);

  tft.begin();
  tft.fillScreen(TFT_BLACK);

  drawProgress(tft, ui, 0, "Initializing filesystem...");
  SPIFFS.begin();

  drawProgress(tft, ui, 10, "Initializing filesystem...");
  listFiles();

  drawProgress(tft, ui, 20, "Calibrating touch...");
  Serial.println("Calibrating touch");
  leftPin.begin(LEFT_TPIN);
  rightPin.begin(RIGHT_TPIN);
  leftPin.calibrate(CALIBRATION_TIME);
  rightPin.calibrate(CALIBRATION_TIME);

  drawProgress(tft, ui, 30, "Connecting to WiFi...");
  
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println();

  udp.begin(localPort);
  syncTime();

  tft.unloadFont();

  ow.partialDataSet(true);
}

void loop() {
  char cmd = 0;
  #ifdef ENABLE_SERIAL_COMMANDS
  if (Serial.available()) {
    cmd = Serial.read();
  }
  #endif

  const bool leftTouched = leftPin.isTouching();
  const bool rightTouched = rightPin.isTouching();

  if (leftTouched) {
    Serial.println("Left touch pin touching");
  }
  if (rightTouched) {
    Serial.println("Right touch pin touching");
  }

  digitalWrite(LED_BUILTIN, leftTouched || rightTouched);

  if (booting || 
      (millis() - lastDownloadUpdate > 1000UL * UPDATE_INTERVAL_SECS) ||
      cmd == 'w') {
    if (!updateData()) {
      Serial.println("Failed to get data, retrying in 1 minute!");
      lastDownloadUpdate = millis() - 1000UL * (UPDATE_INTERVAL_SECS + 60);
      return;
    }
    redraw = true;
  }

  if (cmd == 'r') {
    Serial.println("Flag manual redraw");
    redraw = true;
  } else if (rightTouched || cmd == 'n') {
    Serial.println("Incrementing carousel frame index by 1");
    carouselIndex ++;
    if (carouselIndex > MAX_CAROUSEL_INDEX) {
      carouselIndex = 0;
    }
    redraw = true;
  } else if (leftTouched || cmd == 'p') {
    Serial.println("Decrementing carousel frame index by 1");
    if (carouselIndex > 0) {
      carouselIndex --;
    } else {
      carouselIndex = MAX_CAROUSEL_INDEX;
    }
    redraw = true;
  }

  if (redraw) {
    Serial.println("Redraw flag is true");
    redraw = false;
    Serial.print("carouselIndex is ");
    Serial.println(carouselIndex);
    tft.fillScreen(TFT_BLACK);
    title = carouselTitles[carouselIndex];
    drawTopBar(tft, current, title, carouselIndex, MAX_CAROUSEL_INDEX);
    switch (carouselIndex) {
      case 0: {
        Serial.println("Drawing at a glance frame");
        drawWeatherNow(tft, ui, current, daily, extra);
        break;
      }
      case 1: {
        Serial.println("Drawing hourly forecast frame");
        drawHourlyForecast(tft, ui, current, hourly, extra);
        break;
      }
      case 2: {
        Serial.println("Drawing daily forecast frame");
        drawDailyForecast(tft, ui, current, daily);
        break;
      }
      case 3: {
        Serial.println("Drawing miscellaneous frame");
        drawMiscellaneous(tft, ui, current);
        break;
      }
    }
    lastDownloadUpdate = millis();
  }

  if (booting || minute() != lastMinute || cmd == 't') {
    Serial.println("Redrawing time");
    drawTopBar(tft, current, title, carouselIndex, MAX_CAROUSEL_INDEX);
    lastMinute = minute();
    syncTime();
  }

  #ifdef ENABLE_SERIAL_COMMANDS
  if (booting) {
    Serial.println("Available commands:");
    Serial.println("w: Update weather data and trigger redraw.");
    Serial.println("t: Update time and trigger redraw of time.");
    Serial.println("r: Trigger redraw.");
    Serial.println("n: Next frame in carousel.");
    Serial.println("p: Previous frame in carousel.");
  }
  #endif

  booting = false;
}

bool updateData() {
  const byte loadingCircleRadius = 6;
  const unsigned int loadingCircleX = 240 - loadingCircleRadius - (loadingCircleRadius / 2);
  const unsigned int loadingCircleY = 45 + (loadingCircleRadius / 2) + 4;

  if (booting) {
    drawProgress(tft, ui, 40, "Updating time...");
  } else {
    fillSegment(tft, loadingCircleX, loadingCircleY, 0, 0, loadingCircleRadius, TFT_NAVY);
  }

  if (current != nullptr) {
    delete current;
    delete extra;
    delete hourly;
    delete daily;
  }

  current = new OW_current;
  extra = new OW_extra;
  daily = new OW_daily;
  hourly = new OW_hourly;

  if (booting) {
    drawProgress(tft, ui, 60, "Updating weather...");
  } else {
    fillSegment(tft, loadingCircleX, loadingCircleY, 0, 120, loadingCircleRadius, TFT_NAVY);
  }

  bool parsed = ow.getForecast(current, hourly, daily, api_key, latitude, longitude, units, language);

  if (booting) {
    drawProgress(tft, ui, 80, "Updating weather...");
  } else {
    fillSegment(tft, loadingCircleX, loadingCircleY, 0, 240, loadingCircleRadius, TFT_NAVY);
  }

  parsed = parsed && ow.getExtra(extra, api_key, latitude, longitude, units, language);

  if (parsed) {
    Serial.println("Data received!");
  } else {
    Serial.println("Failed to get data points!");
    return false;
  }

  Serial.print("Free heap: "); Serial.println(ESP.getFreeHeap(), DEC);

  printWeather();

  if (booting) {
    drawProgress(tft, ui, 100, "Done!");
    delay(2000);
  } else {
    fillSegment(tft, loadingCircleX, loadingCircleY, 0, 360, loadingCircleRadius, TFT_NAVY);
    delay(2000);
  }

  return true;
}

void printWeather() {
  Serial.println("Weather from OpenWeather\n");

  Serial.println("############### Current weather ###############\n");
  Serial.print("dt (time)          : "); Serial.println(strDateAndTime(current->dt));
  Serial.print("sunrise            : "); Serial.println(strDateAndTime(current->sunrise));
  Serial.print("sunset             : "); Serial.println(strDateAndTime(current->sunset));
  Serial.print("main               : "); Serial.println(current->main);
  Serial.print("temp               : "); Serial.println(current->temp);
  Serial.print("humidity           : "); Serial.println(current->humidity);
  Serial.print("pressure           : "); Serial.println(current->pressure);
  Serial.print("wind_speed         : "); Serial.println(current->wind_speed);
  Serial.print("wind_deg           : "); Serial.println(current->wind_deg);
  Serial.print("clouds             : "); Serial.println(current->clouds);
  Serial.print("id                 : "); Serial.println(current->id);
  Serial.println();

  Serial.println("###############  Daily weather  ###############\n");
  Serial.println();

  for (int i = 0; i < 5; i++)
  {
    Serial.print("dt (time)          : "); Serial.println(strDateAndTime(daily->dt[i]));
    Serial.print("id                 : "); Serial.println(daily->id[i]);
    Serial.print("temp_max           : "); Serial.println(daily->temp_max[i]);
    Serial.print("temp_min           : "); Serial.println(daily->temp_min[i]);
    Serial.println();
  }

  Serial.println("#################  Extra info  ################\n");
  Serial.print("temp_min           : "); Serial.println(extra->temp_min);
  Serial.print("temp_max           : "); Serial.println(extra->temp_max);
  Serial.print("name               : "); Serial.println(extra->name);
  Serial.print("country            : "); Serial.println(extra->country);
}
