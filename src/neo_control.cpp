#include "neo_control.h"

void neo_control_RPC(void *pvParameters){

    Adafruit_NeoPixel strip(LED_COUNT, NEO_PIN, NEO_GRB + NEO_KHZ800);
    strip.begin();
    // Set all pixels to off to start
    strip.clear();
    strip.show();

    int current_state = 0;

    while(1) {        
        if (xSemaphoreTake(xMutexNeoState, (TickType_t)10) == pdTRUE) 
        {
            current_state = neo_state;
            xSemaphoreGive(xMutexNeoState); 
        } 
        strip.setPixelColor(0, strip.Color(current_state * 255, 0, 0)); // Set pixel 0 to red
        strip.show(); // Update the strip

        uint8_t severity = max((uint8_t)glob_temp_alert_level, (uint8_t)glob_hum_alert_level);

        if (severity == ALERT_NONE) {
            // Blue heartbeat blink
            strip.setPixelColor(0, strip.Color(0, 0, 255));
            strip.show();
            vTaskDelay(500);
            strip.setPixelColor(0, strip.Color(0, 0, 0));
            strip.show();
            vTaskDelay(500);
        } else if (severity == ALERT_WARN) {
            // Warning: amber/orange blink
            strip.setPixelColor(0, strip.Color(255, 165, 0));
            strip.show();
            vTaskDelay(500);
            strip.setPixelColor(0, strip.Color(0, 0, 0));
            strip.show();
            vTaskDelay(500);
        } else { // ALERT_CRIT
            // Critical: fast red blink
            strip.setPixelColor(0, strip.Color(255, 0, 0));
            strip.show();
            vTaskDelay(150);
            strip.setPixelColor(0, strip.Color(0, 0, 0));
            strip.show();
            vTaskDelay(150);
        }
        
        // Wait for 100 milliseconds
        //vTaskDelay(pdMS_TO_TICKS(100));
    }
}