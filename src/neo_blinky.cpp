#include "neo_blinky.h"
#include "global.h"


void neo_blinky(void *pvParameters){

    Adafruit_NeoPixel strip(LED_COUNT, NEO_PIN, NEO_GRB + NEO_KHZ800);
    strip.begin();
    // Set all pixels to off to start
    strip.clear();
    strip.show();

    while(1) {                          
        // Manual override: steady on/off
        if (led2_manual) {
            if (led2_state) {
                strip.setPixelColor(0, strip.Color(255, 255, 255));
            } else {
                strip.setPixelColor(0, strip.Color(0, 0, 0));
            }
            strip.show();
            vTaskDelay(200);
            continue;
        }

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
    }
}