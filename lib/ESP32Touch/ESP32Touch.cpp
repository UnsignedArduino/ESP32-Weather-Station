#include <Arduino.h>
#include "ESP32Touch.h"

ESP32TouchPin::ESP32TouchPin() {

}

bool ESP32TouchPin::begin(touch_pin_t pin) {
  this->_pin = pin;
  return true;
}

#ifdef ESP32_TOUCH_AVERAGE
bool ESP32TouchPin::calibrate(unsigned long time) {
  const unsigned long startTime = millis();
  unsigned long totalValue = 0;
  unsigned long totalTimes = 0;
  while (millis() - startTime < time) {
    totalValue += touchRead(this->_pin);
    totalTimes ++;
  }
  unsigned long average = totalValue / totalTimes;
  this->_threshold = average * ESP32_TOUCH_THRESHOLD_PERCENT;
  #ifdef ESP32_TOUCH_DEBUG
  Serial.print("ESP32TouchPin::calibrate.average[local] = ");
  Serial.println(average);
  Serial.print("ESP32TouchPin::calibrate._threshold = ");
  Serial.println(this->_threshold);
  #endif
  return true;
}
#else
bool ESP32TouchPin::calibrate() {
  this->_threshold = touchRead(this->_pin) * ESP32_TOUCH_THRESHOLD_PERCENT;
  #ifdef ESP32_TOUCH_DEBUG
  Serial.print("ESP32TouchPin::calibrate._threshold = ");
  Serial.println(this->_threshold);
  #endif
  return true;
}
#endif

bool ESP32TouchPin::isTouching() {
  return this->getValue() < this->_threshold;
}

touch_value_t ESP32TouchPin::getValue() {
  #ifdef ESP32_TOUCH_AVERAGE
  unsigned long totalValue = 0;
  for (unsigned int i = 0; i < ESP32_TOUCH_AVERAGE_COUNT; i ++) {
    totalValue += touchRead(this->_pin);
  }
  touch_value_t value = totalValue / ESP32_TOUCH_AVERAGE_COUNT;
  #else
  touch_value_t value = touchRead(this->_pin);
  #endif
  #ifdef ESP32_TOUCH_DEBUG
  Serial.print("ESP32TouchPin::calibrate.value[local] = ");
  Serial.println(value);
  #endif
  return value;
}
