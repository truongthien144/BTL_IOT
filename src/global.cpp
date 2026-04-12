#include "global.h"
float glob_temperature = 0;
float glob_humidity = 0;

// Alert levels (default to none)
volatile AlertLevel glob_temp_alert_level = ALERT_NONE;
volatile AlertLevel glob_hum_alert_level = ALERT_NONE;

// Default thresholds (tweak these values as needed)
const float TEMP_WARN_THRESHOLD = 30.0f;
const float TEMP_CRIT_THRESHOLD = 35.0f;
const float HUM_WARN_THRESHOLD = 60.0f;
const float HUM_CRIT_THRESHOLD = 80.0f;

String WIFI_SSID;
String WIFI_PASS;
String CORE_IOT_TOKEN;
String CORE_IOT_SERVER;
String CORE_IOT_PORT;

String ssid = "ESP32-YOUR NETWORK HERE!!!";
String password = "12345678";
String wifi_ssid = "702H6-KH@KTMT";
String wifi_password = "svkhktmt";
boolean isWifiConnected = false;
SemaphoreHandle_t xBinarySemaphoreInternet = xSemaphoreCreateBinary();

// Manual LED control defaults
bool led1_state = false;
bool led1_manual = false;
bool led2_state = false;
bool led2_manual = false;

// AP mode
bool isAPMode = true;