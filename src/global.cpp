#include "global.h"
float glob_temperature = 0;
float glob_humidity = 0;

// Alert levels (default to none)
volatile AlertLevel glob_temp_alert_level = ALERT_NONE;
volatile AlertLevel glob_hum_alert_level = ALERT_NONE;

// Default thresholds (tweak these values as needed)
const float TEMP_WARN_THRESHOLD = 30.0f;
const float TEMP_CRIT_THRESHOLD = 33.0f;
const float HUM_WARN_THRESHOLD = 60.0f;
const float HUM_CRIT_THRESHOLD = 80.0f;

String WIFI_SSID;
String WIFI_PASS;
String CORE_IOT_TOKEN;
String CORE_IOT_SERVER;
String CORE_IOT_PORT;
String LOCAL_SERVER;

// Local Webserver IP: 192.168.101.84
String ssid = "ESP32-YOUR NETWORK HERE!!!";
String password = "12345678";
String wifi_ssid = "abcde";
String wifi_password = "123456789";
boolean isWifiConnected = false;
SemaphoreHandle_t xBinarySemaphoreInternet = xSemaphoreCreateBinary();

volatile int neo_state = 0; // Global variable to store the state of the NeoPixel
SemaphoreHandle_t xMutexNeoState = xSemaphoreCreateMutex(); // Mutex to protect access to neo_state

SemaphoreHandle_t xSemaphoreLCD = xSemaphoreCreateMutex();

volatile uint16_t blinkingInterval = 1000U; // Global variable to store the LED blinking interval, default is 1000ms
SemaphoreHandle_t xMutexBlinkingInterval = xSemaphoreCreateMutex(); // Mutex to protect access to blinkingInterval