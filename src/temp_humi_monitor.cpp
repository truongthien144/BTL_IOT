#include "temp_humi_monitor.h"

DHT20 dht20;

void temp_humi_monitor(void *pvParameters){

    Wire.begin(11, 12);
    Serial.begin(115200);
    dht20.begin();

    // previous alert levels to detect change and signal semaphores
    AlertLevel prev_temp_level = ALERT_NONE;
    AlertLevel prev_hum_level = ALERT_NONE;

    while (1){
        dht20.read();
        // Reading temperature in Celsius
        float temperature = dht20.getTemperature();
        // Reading humidity
        float humidity = dht20.getHumidity();

        // Check if any reads failed and mark as invalid
        if (isnan(temperature) || isnan(humidity)) {
            Serial.println("Failed to read from DHT sensor!");
            temperature = humidity =  -1;
        }

        //Update global variables for temperature and humidity
        glob_temperature = temperature;
        glob_humidity = humidity;

        // Update alert levels based on thresholds
        if (temperature < 0) {
            glob_temp_alert_level = ALERT_NONE; // sensor error or invalid
        } else if (temperature >= TEMP_CRIT_THRESHOLD) {
            glob_temp_alert_level = ALERT_CRIT;
        } else if (temperature >= TEMP_WARN_THRESHOLD) {
            glob_temp_alert_level = ALERT_WARN;
        } else {
            glob_temp_alert_level = ALERT_NONE;
        }

        if (humidity < 0) {
            glob_hum_alert_level = ALERT_NONE;
        } else if (humidity >= HUM_CRIT_THRESHOLD) {
            glob_hum_alert_level = ALERT_CRIT;
        } else if (humidity >= HUM_WARN_THRESHOLD) {
            glob_hum_alert_level = ALERT_WARN;
        } else {
            glob_hum_alert_level = ALERT_NONE;
        }

        // Logging
        Serial.print("Humidity: ");
        Serial.print(humidity);
        Serial.print("%  Temperature: ");
        Serial.print(temperature);
        Serial.println("°C");

        // Signal LED task when temperature severity changes
        if (glob_temp_alert_level != prev_temp_level)
        {
            if (xSemaphoreLED) xSemaphoreGive(xSemaphoreLED);
            prev_temp_level = glob_temp_alert_level;
            Serial.println("[monitor] temp level changed, signaled LED task");
        }

        // Signal NEO task when humidity severity changes
        if (glob_hum_alert_level != prev_hum_level)
        {
            if (xSemaphoreNEO) xSemaphoreGive(xSemaphoreNEO);
            prev_hum_level = glob_hum_alert_level;
            Serial.println("[monitor] hum level changed, signaled NEO task");
        }

        // Signal LCD task on every measurement update
        if (xSemaphoreLCD) xSemaphoreGive(xSemaphoreLCD);

        vTaskDelay(pdMS_TO_TICKS(3000));
    }

}