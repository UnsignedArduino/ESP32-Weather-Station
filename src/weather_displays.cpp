#include <Arduino.h>
#include <TFT_eSPI.h>
#include "GfxUi.h"
#include "OpenWeather.h"
#include "NTP_Time.h"
#include "weather_displays.h"
#include "All_Settings.h"

void drawTime(TFT_eSPI tft) {
  tft.loadFont(AA_FONT_LARGE);

  // Convert UTC to local time, returns zone code in tz1_Code, e.g "GMT"
  time_t local_time = TIMEZONE.toLocal(now(), &tz1_Code);

  String timeNow = "";

  if (hour(local_time) < 10) timeNow += "0";
  timeNow += hour(local_time);
  timeNow += ":";
  if (minute(local_time) < 10) timeNow += "0";
  timeNow += minute(local_time);

  tft.setTextDatum(TL_DATUM);
  tft.setTextColor(TFT_YELLOW, TFT_BLACK);
  tft.setTextPadding(tft.textWidth(" 44:44 "));  // String width + margin
  tft.drawString(timeNow, 10, 10);

  tft.setTextPadding(0);

  tft.unloadFont();
}

void drawAtAGlance(TFT_eSPI tft, GfxUi ui, OW_current* current, OW_daily* daily, OW_extra* extra) {
  tft.fillScreen(TFT_BLACK);
  drawCurrentWeatherAtAGlance(tft, ui, current, extra);
  drawForecastAtAGlance(tft, ui, current, daily);
}

void drawCurrentWeatherAtAGlance(TFT_eSPI tft, GfxUi ui, OW_current* current, OW_extra* extra) {
  String date = "Updated: " + strDate(current->dt);
  String weatherText = "None";

  tft.loadFont(AA_FONT_SMALL);
  tft.setTextDatum(TL_DATUM);
  tft.setTextColor(TFT_ORANGE, TFT_BLACK);
  tft.setTextPadding(tft.textWidth(" Updated: Mmm 44 44:44 "));  // String width + margin
  tft.drawString(date, 10, 45);

  String weatherIcon = "";

  String currentSummary = current->main;
  currentSummary.toLowerCase();

  weatherIcon = getMeteoconIcon(current, current->id, true);

  //uint32_t dt = millis();
  ui.drawBmp("/icon/" + weatherIcon + ".bmp", 0, 65);
  //Serial.print("Icon draw time = "); Serial.println(millis()-dt);

  // Weather Text
  if (language == "en")
    weatherText = current->main;
  else
    weatherText = current->description;

  tft.setTextDatum(TL_DATUM);
  tft.setTextColor(TFT_ORANGE, TFT_BLACK);

  tft.drawString(extra->name + ", " + extra->country, 105, 75);

  // int splitPoint = 0;
  // splitPoint = splitIndex(weatherText);

  // tft.setTextPadding(xpos - 100);  // xpos - icon width
  /*if (splitPoint) */tft.drawString(weatherText/*.substring(0, splitPoint)*/, 105, 91);
  //else tft.drawString(" ", 55, 81);
  // tft.drawString(weatherText.substring(splitPoint), xpos, 83);

  tft.unloadFont();

  tft.loadFont(AA_FONT_LARGE);
  tft.setTextDatum(TL_DATUM);
  tft.setTextColor(TFT_YELLOW, TFT_BLACK);

  // Font ASCII code 0xB0 is a degree symbol, but o used instead in small font
  tft.setTextPadding(tft.textWidth(" -88")); // Max width of values

  weatherText = String(current->temp, 0);  // Make it integer temperature
  const unsigned int tempSize = tft.drawString(weatherText, 105, 111); //  + "°" symbol is big... use o in small font
  tft.unloadFont();

  tft.loadFont(AA_FONT_SMALL);

  unsigned int degreeSize;

  tft.setTextPadding(0);
  if (units == "metric") degreeSize = tft.drawString("oC", 110 + tempSize, 113);
  else degreeSize = tft.drawString("oF", 110 + tempSize, 113);

  tft.setTextColor(TFT_ORANGE, TFT_BLACK);
  const unsigned int humidityOffset = tft.drawString("Humidity: ", 105, 145);

  String humidity = "";
  humidity += current->humidity;
  humidity += "%";

  tft.setTextDatum(TL_DATUM);
  tft.setTextColor(TFT_YELLOW, TFT_BLACK);
  tft.setTextPadding(tft.textWidth("100%"));
  tft.drawString(humidity, 105 + humidityOffset, 145);

  tft.setTextColor(TFT_WHITE, TFT_BLACK);

  weatherText = String(extra->temp_max, 0);
  tft.drawString(weatherText, 110 + tempSize + degreeSize + 5, 113);

  weatherText = String(extra->temp_min, 0);
  tft.drawString(weatherText, 110 + tempSize + degreeSize + 5, 129);

  drawSeparator(tft, 65);

  tft.setTextColor(TFT_ORANGE, TFT_BLACK);
  weatherText = String(current->wind_speed, 0);

  if (units == "metric") weatherText += " m/s";
  else weatherText += " mph ";

  int windAngle = (current->wind_deg + 22.5) / 45;
  if (windAngle > 7) windAngle = 0;
  String wind[] = {"N", "NE", "E", "SE", "S", "SW", "W", "NW" };
  // ui.drawBmp("/wind/" + wind[windAngle] + ".bmp", 101, 98);
  weatherText += wind[windAngle];

  const unsigned int windOffset = tft.drawString("Wind: ", 10, 170);

  tft.setTextColor(TFT_YELLOW, TFT_BLACK);

  tft.setTextDatum(TL_DATUM);
  tft.setTextPadding(tft.textWidth("888 m/s NW")); // Max string length?
  tft.drawString(weatherText, 10 + windOffset, 170);

  tft.setTextColor(TFT_ORANGE, TFT_BLACK);
  weatherText = String(current->uvi, 0);

  const unsigned int uvOffset = tft.drawString("UV index: ", 10, 188);

  // https://www.epa.gov/sunsafety/uv-index-scale-0
  if (current->uvi >= 11) {
    tft.setTextColor(TFT_PURPLE, TFT_BLACK);
    weatherText += " (extreme)";
  } else if (current->uvi >= 8) {
    tft.setTextColor(TFT_RED, TFT_BLACK);
    weatherText += " (very high)";
  } else if (current->uvi >= 6) {
    tft.setTextColor(TFT_ORANGE, TFT_BLACK);
    weatherText += " (high)";
  } else if (current->uvi >= 3) {
    tft.setTextColor(TFT_YELLOW, TFT_BLACK);
    weatherText += " (moderate)";
  } else {
    tft.setTextColor(TFT_GREEN, TFT_BLACK);
    weatherText += " (low)";
  }

  tft.setTextDatum(TL_DATUM);
  tft.setTextPadding(tft.textWidth("11")); // Max string length?
  tft.drawString(weatherText, 10 + uvOffset, 188);

  // if (units == "imperial")
  // {
  //   weatherText = current->pressure * 0.02953;
  //   weatherText += " in";
  // }
  // else
  // {
  //   weatherText = String(current->pressure, 0);
  //   weatherText += " hPa";
  // }

  // tft.setTextDatum(TR_DATUM);
  // tft.setTextPadding(tft.textWidth(" 8888hPa")); // Max string length?
  // tft.drawString(weatherText, 230, 148);

  drawSeparator(tft, 165);

  tft.setTextDatum(TL_DATUM); // Reset datum to normal
  tft.setTextPadding(0);      // Reset padding width to none
  tft.unloadFont();
}

void drawForecastAtAGlance(TFT_eSPI tft, GfxUi ui, OW_current* current, OW_daily* daily) {
  drawSeparator(tft, 231);

  int8_t dayIndex = 1;

  tft.loadFont(AA_FONT_SMALL);
  drawForecastDetailAtAGlance(tft, ui, current, daily, 8, 250, dayIndex ++);
  drawForecastDetailAtAGlance(tft, ui, current, daily, 66, 250, dayIndex ++);
  drawForecastDetailAtAGlance(tft, ui, current, daily, 124, 250, dayIndex ++);
  drawForecastDetailAtAGlance(tft, ui, current, daily, 182, 250, dayIndex);
  tft.unloadFont();
}

void drawForecastDetailAtAGlance(TFT_eSPI tft, GfxUi ui, OW_current* current, OW_daily* daily, uint16_t x, uint16_t y, uint8_t dayIndex) {
  if (dayIndex >= MAX_DAYS) return;

  String day = shortDOW[weekday(TIMEZONE.toLocal(daily->dt[dayIndex], &tz1_Code))];
  day.toUpperCase();

  tft.setTextDatum(BC_DATUM);

  tft.setTextColor(TFT_ORANGE, TFT_BLACK);
  tft.setTextPadding(tft.textWidth("WWW"));
  tft.drawString(day, x + 25, y);
  Serial.print("Day: ");
  Serial.print(day);

  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextPadding(tft.textWidth("-88   -88"));
  String highTemp = String(daily->temp_max[dayIndex], 0);
  String lowTemp  = String(daily->temp_min[dayIndex], 0);
  tft.drawString(highTemp + " " + lowTemp, x + 25, y + 17);

  Serial.print(" high: "); Serial.print(highTemp);
  Serial.print(" low: "); Serial.println(lowTemp);

  String weatherIcon = getMeteoconIcon(current, daily->id[dayIndex], false);

  ui.drawBmp("/icon50/" + weatherIcon + ".bmp", x, y + 18);

  tft.setTextPadding(0); // Reset padding width to none
}

const char* getMeteoconIcon(OW_current* current, uint16_t id, bool today) {
  if ( today && id/100 == 8 && (current->dt < current->sunrise || current->dt > current->sunset)) id += 1000; 

  if (id/100 == 2) return "thunderstorm";
  if (id/100 == 3) return "drizzle";
  if (id/100 == 4) return "unknown";
  if (id == 500) return "lightRain";
  else if (id == 511) return "sleet";
  else if (id/100 == 5) return "rain";
  if (id >= 611 && id <= 616) return "sleet";
  else if (id/100 == 6) return "snow";
  if (id/100 == 7) return "fog";
  if (id == 800) return "clear-day";
  if (id == 801) return "partly-cloudy-day";
  if (id == 802) return "cloudy";
  if (id == 803) return "cloudy";
  if (id == 804) return "cloudy";
  if (id == 1800) return "clear-night";
  if (id == 1801) return "partly-cloudy-night";
  if (id == 1802) return "cloudy";
  if (id == 1803) return "cloudy";
  if (id == 1804) return "cloudy";

  return "unknown";
}

void drawProgress(TFT_eSPI tft, GfxUi ui, uint8_t percentage, String text) {
  tft.loadFont(AA_FONT_SMALL);
  tft.setTextDatum(BC_DATUM);
  tft.setTextColor(TFT_ORANGE, TFT_BLACK);
  tft.setTextPadding(240);
  tft.drawString(text, 120, 260);

  ui.drawProgressBar(10, 269, 240 - 20, 15, percentage, TFT_WHITE, TFT_BLUE);

  tft.setTextPadding(0);
  tft.unloadFont();
}

void drawSeparator(TFT_eSPI tft, uint16_t y) {
  tft.drawFastHLine(10, y, 240 - 2 * 10, 0x4228);
}

int rightOffset(TFT_eSPI tft, String text, String sub) {
  int index = text.indexOf(sub);
  return tft.textWidth(text.substring(index));
}

int leftOffset(TFT_eSPI tft, String text, String sub) {
  int index = text.indexOf(sub);
  return tft.textWidth(text.substring(0, index));
}

void fillSegment(TFT_eSPI tft, int x, int y, int start_angle, int sub_angle, int r, unsigned int colour) {
  // Calculate first pair of coordinates for segment start
  float sx = cos((start_angle - 90) * DEG2RAD);
  float sy = sin((start_angle - 90) * DEG2RAD);
  uint16_t x1 = sx * r + x;
  uint16_t y1 = sy * r + y;

  // Draw colour blocks every INC degrees
  for (int i = start_angle; i < start_angle + sub_angle; i += INC) {

    // Calculate pair of coordinates for segment end
    int x2 = cos((i + 1 - 90) * DEG2RAD) * r + x;
    int y2 = sin((i + 1 - 90) * DEG2RAD) * r + y;

    tft.fillTriangle(x1, y1, x2, y2, x, y, colour);

    // Copy segment end to segment start for next segment
    x1 = x2;
    y1 = y2;
  }
}

String strTime(time_t unixTime) {
  time_t local_time = TIMEZONE.toLocal(unixTime, &tz1_Code);

  String localTime = "";

  if (hour(local_time) < 10) localTime += "0";
  localTime += hour(local_time);
  localTime += ":";
  if (minute(local_time) < 10) localTime += "0";
  localTime += minute(local_time);

  return localTime;
}

String strDate(time_t unixTime) {
  time_t local_time = TIMEZONE.toLocal(unixTime, &tz1_Code);

  String localDate = "";

  localDate += monthShortStr(month(local_time));
  localDate += " ";
  localDate += day(local_time);
  localDate += " " + strTime(unixTime);

  return localDate;
}