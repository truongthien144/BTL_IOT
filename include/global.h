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

// legacy / additional wifi vars used in various modules
extern String ssid;         // AP SSID
extern String password;     // AP password
extern String wifi_ssid;    // configured STA SSID (duplicate/alias used elsewhere)
extern String wifi_password;// configured STA password (duplicate/alias used elsewhere)

// LED control globals
extern bool led1_state;     // manual on/off state for LED1 (led_blinky)
extern bool led1_manual;    // when true, manual override is active for LED1
extern bool led2_state;     // manual on/off state for LED2 (neo)
extern bool led2_manual;    // when true, manual override is active for LED2

// AP mode indicator
extern bool isAPMode;

extern boolean isWifiConnected;
extern SemaphoreHandle_t xBinarySemaphoreInternet;
// Semaphores for task synchronization (Task requirements)
extern SemaphoreHandle_t xSemaphoreLED;   // signaled when temperature condition changes
extern SemaphoreHandle_t xSemaphoreNEO;   // signaled when humidity condition changes
extern SemaphoreHandle_t xSemaphoreLCD;   // signaled when new sensor data is available for display
#endif