#include "led_blinky.h"

void led_blinky(void *pvParameters){
  pinMode(LED_GPIO, OUTPUT);

  while(1) {
    // Wait until monitor signals a temperature condition change
    if (xSemaphoreTake(xSemaphoreLED, portMAX_DELAY) == pdTRUE)
    {
      // If manual override is active, honor manual state instead
      if (led1_manual)
      {
        digitalWrite(LED_GPIO, led1_state ? HIGH : LOW);
        // keep the manual state visible for a while
        vTaskDelay(pdMS_TO_TICKS(1000));
        continue;
      }

      // Read current temperature severity
      uint8_t severity = (uint8_t)glob_temp_alert_level;

      // Execute pattern for a few cycles to make the change visible
      int cycles = 6; // tunable
      for (int c = 0; c < cycles; ++c)
      {
        if (severity == ALERT_NONE)
        {
          // heartbeat double blink
          digitalWrite(LED_GPIO, HIGH);
          vTaskDelay(pdMS_TO_TICKS(200));
          digitalWrite(LED_GPIO, LOW);
          vTaskDelay(pdMS_TO_TICKS(200));
          digitalWrite(LED_GPIO, HIGH);
          vTaskDelay(pdMS_TO_TICKS(200));
          digitalWrite(LED_GPIO, LOW);
          vTaskDelay(pdMS_TO_TICKS(1000));
        }
        else if (severity == ALERT_WARN)
        {
          digitalWrite(LED_GPIO, HIGH);
          vTaskDelay(pdMS_TO_TICKS(500));
          digitalWrite(LED_GPIO, LOW);
          vTaskDelay(pdMS_TO_TICKS(500));
        }
        else // ALERT_CRIT
        {
          digitalWrite(LED_GPIO, HIGH);
          vTaskDelay(pdMS_TO_TICKS(150));
          digitalWrite(LED_GPIO, LOW);
          vTaskDelay(pdMS_TO_TICKS(150));
        }
      }
    }
  }
}