#include <Arduino.h>
#include <TFT_eSPI.h>
#include "GfxUi.h"
#include "OpenWeather.h"

#define AA_FONT_SMALL "fonts/NotoSansBold15"
#define AA_FONT_LARGE "fonts/NotoSansBold36"

void drawTopBar(TFT_eSPI tft, OW_current* current, String title, byte currentFrame, byte maxFrame);

void drawWeatherNow(TFT_eSPI tft, GfxUi ui, OW_current* current, OW_daily* daily, OW_extra* extra);
void drawCurrentWeatherNow(TFT_eSPI tft, GfxUi ui, OW_current* current, OW_extra* extra);
void drawForecastWeatherNow(TFT_eSPI tft, GfxUi ui, OW_current* current, OW_daily* daily);
void drawDetailForecastWeatherNow(TFT_eSPI tft, GfxUi ui, OW_current* current, OW_daily* daily, uint16_t x, uint16_t y, uint8_t dayIndex);

void drawHourlyForecast(TFT_eSPI tft, GfxUi ui, OW_current* current, OW_hourly* hourly, OW_extra* extra);
void drawDetailHourlyForecast(TFT_eSPI tft, GfxUi ui, OW_current* current, OW_hourly* hourly, OW_extra* extra, uint16_t x, uint16_t y, byte hrIndex);

void drawDailyForecast(TFT_eSPI tft, GfxUi ui, OW_current* current, OW_daily* daily);
void drawDetailDailyForecast(TFT_eSPI tft, GfxUi ui, OW_current* current, OW_daily* daily, uint16_t x, uint16_t y, uint8_t dayIndex);

void drawMiscellaneous(TFT_eSPI tft, GfxUi ui, OW_current* current);
void drawMiscellaneousLabelValue(TFT_eSPI tft, String label, String value, uint16_t x, uint16_t y);

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
String strDateAndTime(time_t unixTime);
