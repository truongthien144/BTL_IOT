#include "neo_blinky.h"
#include "global.h"


void neo_blinky(void *pvParameters){

    Adafruit_NeoPixel strip(LED_COUNT, NEO_PIN, NEO_GRB + NEO_KHZ800);
    strip.begin();
    // Set all pixels to off to start
    strip.clear();
    strip.show();

    while(1) {
        // Wait for a humidity change signal
        if (xSemaphoreTake(xSemaphoreNEO, portMAX_DELAY) == pdTRUE)
        {
            // honor manual override
            if (led2_manual)
            {
                if (led2_state)
                    strip.setPixelColor(0, strip.Color(255, 255, 255));
                else
                    strip.setPixelColor(0, strip.Color(0, 0, 0));
                strip.show();
                vTaskDelay(pdMS_TO_TICKS(1000));
                continue;
            }

            // Choose color/pattern based on humidity severity
            uint8_t severity = (uint8_t)glob_hum_alert_level;

            int cycles = 6;
            for (int c = 0; c < cycles; ++c)
            {
                if (severity == ALERT_NONE)
                {
                    // Normal: blue slow pulse
                    strip.setPixelColor(0, strip.Color(0, 0, 200));
                    strip.show();
                    vTaskDelay(pdMS_TO_TICKS(800));
                    strip.setPixelColor(0, strip.Color(0, 0, 0));
                    strip.show();
                    vTaskDelay(pdMS_TO_TICKS(400));
                }
                else if (severity == ALERT_WARN)
                {
                    // Warning: amber
                    strip.setPixelColor(0, strip.Color(255, 165, 0));
                    strip.show();
                    vTaskDelay(pdMS_TO_TICKS(500));
                    strip.setPixelColor(0, strip.Color(0, 0, 0));
                    strip.show();
                    vTaskDelay(pdMS_TO_TICKS(300));
                }
                else // ALERT_CRIT
                {
                    // Critical: red fast blink
                    strip.setPixelColor(0, strip.Color(255, 0, 0));
                    strip.show();
                    vTaskDelay(pdMS_TO_TICKS(200));
                    strip.setPixelColor(0, strip.Color(0, 0, 0));
                    strip.show();
                    vTaskDelay(pdMS_TO_TICKS(150));
                }
            }
        }
    }
}