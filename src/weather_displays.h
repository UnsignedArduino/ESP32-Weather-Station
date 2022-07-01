#include <Arduino.h>
#include <TFT_eSPI.h>
#include "GfxUi.h"
#include "OpenWeather.h"

#define AA_FONT_SMALL "fonts/NotoSansBold15"
#define AA_FONT_LARGE "fonts/NotoSansBold36"

void drawTime(TFT_eSPI tft, OW_current* current);

void drawAtAGlance(TFT_eSPI tft, GfxUi ui, OW_current* current, OW_daily* daily, OW_extra* extra);

void drawCurrentWeatherAtAGlance(TFT_eSPI tft, GfxUi ui, OW_current* current, OW_extra* extra);
void drawForecastAtAGlance(TFT_eSPI tft, GfxUi ui, OW_current* current, OW_daily* daily);
void drawForecastDetailAtAGlance(TFT_eSPI tft, GfxUi ui, OW_current* current, OW_daily* daily, uint16_t x, uint16_t y, uint8_t dayIndex);

void drawHourlyWeather(TFT_eSPI tft, GfxUi ui, OW_current* current, OW_hourly* hourly, OW_extra* extra);
void drawDetailHourlyWeather(TFT_eSPI tft, GfxUi ui, OW_current* current, OW_hourly* hourly, OW_extra* extra, uint16_t x, uint16_t y, byte hrIndex);

const char* getMeteoconIcon(OW_current* current, uint16_t id, bool today);

void drawProgress(TFT_eSPI tft, GfxUi ui, uint8_t percentage, String text);
void drawHSeparator(TFT_eSPI tft, uint16_t y);
void drawVSeparator(TFT_eSPI tft, uint16_t x);

int rightOffset(TFT_eSPI tft, String text, String sub);
int leftOffset(TFT_eSPI tft, String text, String sub);

#define DEG2RAD 0.0174532925 // Degrees to Radians conversion factor
#define INC 2 // Minimum segment subtended angle and plotting angle increment (in degrees)
void fillSegment(TFT_eSPI tft, int x, int y, int start_angle, int sub_angle, int r, unsigned int colour);

String strTime(time_t unixTime);
String strDate(time_t unixTime);
