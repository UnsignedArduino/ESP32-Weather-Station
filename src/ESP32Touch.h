#include <Arduino.h>

typedef uint8_t touch_pin_t;

#ifndef ESP32_TOUCH_AVERAGE
#define ESP32_TOUCH_AVERAGE
#endif
#if defined(ESP32_TOUCH_AVERAGE) && !defined(ESP32_TOUCH_AVERAGE_COUNT)
#define ESP32_TOUCH_AVERAGE_COUNT 100
#endif
#ifndef ESP32_TOUCH_THRESHOLD_PERCENT
#define ESP32_TOUCH_THRESHOLD_PERCENT 0.75;
#endif

// #define ESP32_TOUCH_DEBUG

class ESP32TouchPin {
  public:
    ESP32TouchPin();

    bool begin(touch_pin_t pin);

    #ifdef ESP32_TOUCH_AVERAGE
    bool calibrate(unsigned long time);
    #else
    bool calibrate();
    #endif

    bool isTouching();

    touch_value_t getValue();

  private:
    touch_pin_t _pin = 0;

    touch_value_t _threshold = 0;
};
