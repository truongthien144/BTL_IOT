#ifndef __RFID_TASK__
#define __RFID_TASK__
#include "global.h"
#include <Arduino.h>
#include <SPI.h>
#include <MFRC522.h>
#include <ESP32Servo.h>

// ================== PIN CHO YOLO UNO ==================
#define RST_PIN     6     // GPIO6  = D3
#define SS_PIN      5     // GPIO5  = D2   (SDA/CS)
#define SERVO_PIN   18    // GPIO18 = D9

// SPI custom pins
#define RFID_SCK_PIN   17   // GPIO17 = D8
#define RFID_MOSI_PIN  21   // GPIO21 = D10
#define RFID_MISO_PIN  38   // GPIO38 = D11
// =====================================================

void rfid_task(void *pvParameters);

#endif