#ifndef __GLOBAL_H__
#define __GLOBAL_H__

#include <Arduino.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

extern float glob_temperature;
extern float glob_humidity;
// Temperature / Humidity alert levels
// 0 = NONE, 1 = WARNING, 2 = CRITICAL
enum AlertLevel : uint8_t { ALERT_NONE = 0, ALERT_WARN = 1, ALERT_CRIT = 2 };

extern volatile AlertLevel glob_temp_alert_level;
extern volatile AlertLevel glob_hum_alert_level;

extern const float TEMP_WARN_THRESHOLD;    // e.g. 30.0 C
extern const float TEMP_CRIT_THRESHOLD;    // e.g. 35.0 C
extern const float HUM_WARN_THRESHOLD;     // e.g. 70.0 %
extern const float HUM_CRIT_THRESHOLD;     // e.g. 85.0 %

extern String WIFI_SSID;
extern String WIFI_PASS;
extern String CORE_IOT_TOKEN;
extern String CORE_IOT_SERVER;
extern String CORE_IOT_PORT;
extern String LOCAL_SERVER;

extern boolean isWifiConnected;
extern SemaphoreHandle_t xBinarySemaphoreInternet;

extern volatile int neo_state;
extern SemaphoreHandle_t xMutexNeoState;

extern SemaphoreHandle_t xSemaphoreLCD;   // signaled when new sensor data is available for display

extern volatile uint16_t blinkingInterval;
extern SemaphoreHandle_t xMutexBlinkingInterval;
#endif