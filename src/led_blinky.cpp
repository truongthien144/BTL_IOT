#include "led_blinky.h"

void led_blinky(void *pvParameters){
  pinMode(LED_GPIO, OUTPUT);
  
  while(1) {                        
    // If manual override is active, set LED according to manual state
    if (led1_manual) {
      digitalWrite(LED_GPIO, led1_state ? HIGH : LOW);
      vTaskDelay(200);
      continue;
    }

    // Choose blink behavior based on highest alert severity
    uint8_t severity = max((uint8_t)glob_temp_alert_level, (uint8_t)glob_hum_alert_level);

    if (severity == ALERT_NONE) {
      // Quick double blink (heartbeat effect)
      digitalWrite(LED_GPIO, HIGH);
      vTaskDelay(200);
      digitalWrite(LED_GPIO, LOW);
      vTaskDelay(200);

      digitalWrite(LED_GPIO, HIGH);
      vTaskDelay(200);
      digitalWrite(LED_GPIO, LOW);

      // Pause before repeating
      vTaskDelay(1000);
    } else if (severity == ALERT_WARN) {
      // Warning: moderate blink (slow attention)
      digitalWrite(LED_GPIO, HIGH);
      vTaskDelay(500);
      digitalWrite(LED_GPIO, LOW);
      vTaskDelay(500);
    } else { // ALERT_CRIT
      // Critical: rapid blink
      digitalWrite(LED_GPIO, HIGH);
      vTaskDelay(150);
      digitalWrite(LED_GPIO, LOW);
      vTaskDelay(150);
    }
  }
}