#include "led_blinky.h"

void led_blinky(void *pvParameters){
  pinMode(LED_GPIO, OUTPUT);
  
  int current_blinking_interval = 1000U;
  while(1) {
    if(xSemaphoreTake(xMutexBlinkingInterval, (TickType_t)10) == pdTRUE) 
    {
        // ----- (CRITICAL SECTION) -----
        current_blinking_interval = blinkingInterval;
        // return the mutex after updating the state
         
        // ------------------------------------------------
    } 
    else 
    {
        Serial.println("⚠️ ERROR: cannot get Mutex, skip reading blinking interval!");
    }

    // Choose blink behavior based on highest alert severity
    uint8_t severity = max((uint8_t)glob_temp_alert_level, (uint8_t)glob_hum_alert_level);

    if (severity == ALERT_NONE) {
      // Quick double blink (heartbeat effect)
      digitalWrite(LED_GPIO, HIGH);
      vTaskDelay(current_blinking_interval/4);
      digitalWrite(LED_GPIO, LOW);
      vTaskDelay(current_blinking_interval/4);

      digitalWrite(LED_GPIO, HIGH);
      vTaskDelay(current_blinking_interval/4);
      digitalWrite(LED_GPIO, LOW);

      // Pause before repeating
      vTaskDelay(current_blinking_interval);
    } else if (severity == ALERT_WARN) {
      // Warning: moderate blink (slow attention)
      digitalWrite(LED_GPIO, HIGH);
      vTaskDelay(current_blinking_interval/2);
      digitalWrite(LED_GPIO, LOW);
      vTaskDelay(current_blinking_interval/2);
    } else { // ALERT_CRIT
      // Critical: rapid blink
      digitalWrite(LED_GPIO, HIGH);
      vTaskDelay(current_blinking_interval/5);
      digitalWrite(LED_GPIO, LOW);
      vTaskDelay(current_blinking_interval/5);
    }
    xSemaphoreGive(xMutexBlinkingInterval);
  }
}