#include "task_lcd.h"

// Typical I2C address; adjust to your module if needed
#include <Wire.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

void task_lcd(void *pvParameters)
{
    // initialize I2C pins - temp_humi_monitor already calls Wire.begin; safe to call again
    Wire.begin(11, 12);
    lcd.begin();
    lcd.backlight();

    const TickType_t timeout = pdMS_TO_TICKS(5000);

    while (1)
    {
        // Wait until monitor signals new data or timeout to refresh
        if (xSemaphoreTake(xSemaphoreLCD, timeout) == pdTRUE)
        {
            // update immediately on event
        }

        // Decide state based on the highest severity
        AlertLevel state = (AlertLevel)max((uint8_t)glob_temp_alert_level, (uint8_t)glob_hum_alert_level);

        lcd.clear();
        if (state == ALERT_NONE)
        {
            // Normal: show temperature and humidity
            char buf[32];
            if (isWifiConnected)
            {
                snprintf(buf, sizeof(buf), "T: %.2f H:%.2f", glob_temperature, glob_humidity);
            }
            else
            {
                snprintf(buf, sizeof(buf), "T: N/A   H: N/A");
            }
            lcd.setCursor(0, 0);
            lcd.print(buf);
            lcd.setCursor(0, 1);
            lcd.print("Status: NORMAL   ");
        }
        else if (state == ALERT_WARN)
        {
            char buf[32];
            snprintf(buf, sizeof(buf), "T: %.2f H:%.2f", glob_temperature, glob_humidity);
            lcd.setCursor(0, 0);
            lcd.print(buf);
            lcd.setCursor(0, 1);
            lcd.print("Status: WARNING  ");
        }
        else // ALERT_CRIT
        {
            char buf[32];
            snprintf(buf, sizeof(buf), "T: %.2f H:%.2f", glob_temperature, glob_humidity);
            lcd.setCursor(0, 0);
            lcd.print(buf);
            lcd.setCursor(0, 1);
            lcd.print("Status: CRITICAL ");
        }
        xSemaphoreGive(xSemaphoreLCD);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}