#include "temp_humi_monitor.h"
DHT20 dht20;
LiquidCrystal_I2C lcd(33,16,2);


void temp_humi_monitor(void *pvParameters){

    Wire.begin(11, 12);
    Serial.begin(115200);
    dht20.begin();

    while (1){
        /* code */
        
        dht20.read();
        // Reading temperature in Celsius
        float temperature = dht20.getTemperature();
        // Reading humidity
        float humidity = dht20.getHumidity();

        

        // Check if any reads failed and exit early
        if (isnan(temperature) || isnan(humidity)) {
            Serial.println("Failed to read from DHT sensor!");
            temperature = humidity =  -1;
            //return;
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

        // Print the results
        
        Serial.print("Humidity: ");
        Serial.print(humidity);
        Serial.print("%  Temperature: ");
        Serial.print(temperature);
        Serial.println("°C");
        
        vTaskDelay(5000);
    }
    
}