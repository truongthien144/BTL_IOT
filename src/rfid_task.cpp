#include "rfid_task.h"

MFRC522 rfid(SS_PIN, RST_PIN);
Servo myServo;

// 3 valid UIDs
byte authorizedUIDs[3][4] = {
  {0x2E, 0xB9, 0x75, 0x06},
  {0x51, 0x29, 0x3B, 0x02},
  {0x71, 0x64, 0x0E, 0x06}
};

bool isAuthorized() {
  if (rfid.uid.size != 4) return false;
  for (int i = 0; i < 3; i++) {
    if (memcmp(rfid.uid.uidByte, authorizedUIDs[i], 4) == 0) {
      return true;
    }
  }
  return false;
}

void printUID() {
  Serial.print("UID: ");
  for (byte i = 0; i < rfid.uid.size; i++) {
    if (rfid.uid.uidByte[i] < 0x10) Serial.print("0");
    Serial.print(rfid.uid.uidByte[i], HEX);
    Serial.print(" ");
  }
  Serial.print("(Size: ");
  Serial.print(rfid.uid.size);
  Serial.println(" bytes)");
}

void openDoor() {
  Serial.println(">>> TRUY CẬP HỢP LỆ - MỞ CỬA <<<");
  myServo.write(90);
  vTaskDelay(3000 / portTICK_PERIOD_MS);  // Hold the door open for 3 seconds
  myServo.write(0);
  Serial.println(">>> CỬA ĐÃ ĐÓNG LẠI <<<\n");
}

void denyAccess() {
  Serial.println(">>> TRUY CẬP BỊ TỪ CHỐI - THẺ KHÔNG HỢP LỆ <<<\n");
}



void rfid_task(void *pvParameters) {
    Serial.begin(115200);
    vTaskDelay(2000 / portTICK_PERIOD_MS);  
    
    // Check wiring by attempting to initialize RFID reader
    while (!Serial)
    Serial.println("Nối dây sai.");
    // Initialize RFID reader and SPI pins
    SPI.begin(RFID_SCK_PIN, RFID_MISO_PIN, RFID_MOSI_PIN, SS_PIN);
    rfid.PCD_Init();

    myServo.attach(SERVO_PIN);
    myServo.write(0);
    
        while (1)
        {
            if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial()) {
                vTaskDelay(150 / portTICK_PERIOD_MS);
                continue;  // Exit the loop if no new card is present or if reading fails
            }
                    // === Check RFID (if new card is present) ===  
            Serial.println("--- ĐÃ PHÁT HIỆN THẺ MỚI (RFID) ---");
            printUID();

            if (isAuthorized()) {
                if (rfid.uid.uidByte[0] == 0x2E && 
                rfid.uid.uidByte[1] == 0xB9 && 
                rfid.uid.uidByte[2] == 0x75 && 
                rfid.uid.uidByte[3] == 0x06) {
                    Serial.println(">>> CHÀO ANH A <<<");
                }
                else if (rfid.uid.uidByte[0] == 0x51 && 
                rfid.uid.uidByte[1] == 0x29 && 
                rfid.uid.uidByte[2] == 0x3B && 
                rfid.uid.uidByte[3] == 0x02) {
                    Serial.println(">>> CHÀO ANH B <<<");
                }
                else if (rfid.uid.uidByte[0] == 0x71 && 
                rfid.uid.uidByte[1] == 0x64 && 
                rfid.uid.uidByte[2] == 0x0E && 
                rfid.uid.uidByte[3] == 0x06) {
                    Serial.println(">>> CHÀO ANH C <<<");
                }
                openDoor();
            } else {
                denyAccess();
            }

            rfid.PICC_HaltA();
            rfid.PCD_StopCrypto1();
            vTaskDelay(5000 / portTICK_PERIOD_MS);
        }
}