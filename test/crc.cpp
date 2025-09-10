#include <Arduino.h>
uint16_t modbusCRC(const uint8_t *buf, uint16_t len);
size_t buildModbusWrite(uint8_t slaveID, uint16_t regAddr, uint16_t value, uint8_t *frame);
String decToHex(uint32_t value, uint8_t width = 0);
void toggleToinv();
void valueToinv()

// คำนวณ CRC16 (Modbus)
uint16_t modbusCRC(const uint8_t *buf, uint16_t len) {
  uint16_t crc = 0xFFFF;
  for (uint16_t pos = 0; pos < len; pos++) {
    crc ^= (uint16_t)buf[pos];
    for (uint8_t i = 0; i < 8; i++) {
      if (crc & 0x0001) {
        crc >>= 1;
        crc ^= 0xA001;
      } else {
        crc >>= 1;
      }
    }
  }
  return crc;
}

// ฟังก์ชันสร้าง Modbus Write Single Register (0x06)
size_t buildModbusWrite(uint8_t slaveID, uint16_t regAddr, uint16_t value, uint8_t *frame) {
  frame[0] = slaveID;         // Slave ID
  frame[1] = 0x06;            // Function code (Write Single Register)
  frame[2] = regAddr >> 8;    // Register High
  frame[3] = regAddr & 0xFF;  // Register Low
  frame[4] = value >> 8;      // Value High
  frame[5] = value & 0xFF;    // Value Low

  uint16_t crc = modbusCRC(frame, 6);
  frame[6] = crc & 0xFF;      // CRC Low
  frame[7] = crc >> 8;        // CRC High

  return 8; // frame length
}

// ตัวอย่างการใช้งาน

String decToHex(uint32_t value, uint8_t width = 0) {
  char buffer[16];
  if (width > 0) {
    sprintf(buffer, "%0*X", width, value);  // ใส่ 0 เติมด้านหน้า
  } else {
    sprintf(buffer, "%X", value);           // ไม่มี padding
  }
  return String(buffer);
}

void toggleToinv() {
  Serial.begin(115200);

  uint8_t frame[8];
  size_t len = buildModbusWrite(0x05, 0x13AB, 0x0002, frame); // ตั้งค่า Grid-tie current = 4A

  Serial.print("Command HEX: ");
  for (size_t i = 0; i < len; i++) {
    if (frame[i] < 0x10) Serial.print("0");
    Serial.print(frame[i], HEX);
    Serial.print(" ");
  }
  Serial.println();
}

void valueToinv(String Name,String val) {
  Serial.begin(115200);

  uint8_t frame[8];
  size_t len = buildModbusWrite(0x05, 0x139e, 0x0002, frame); // ตั้งค่า Grid-tie current = 4A

  Serial.print("Command HEX: ");
  for (size_t i = 0; i < len; i++) {
    if (frame[i] < 0x10) Serial.print("0");
    Serial.print(frame[i], HEX);
    Serial.print(" ");
  }
  Serial.println();
}