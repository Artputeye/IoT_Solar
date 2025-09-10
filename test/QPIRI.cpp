#include <Arduino.h>

// สร้าง struct สำหรับเก็บค่า QPIRI
struct QPIRI_Data {
  float acVoltage;        // 230.0
  float acCurrent;        // 18.2
  float acVoltage2;       // 230.0
  float acFrequency;      // 50.0
  float acCurrent2;       // 18.2
  unsigned long ratedPower;   // 4200
  unsigned long maxPower;     // 4200
  float busVoltage;        // 24.0
  float batteryVoltage;    // 23.0
  float lowBatteryCutoff;  // 20.0
  float maxChargeVoltage;  // 28.2
  float floatVoltage;      // 27.0
  int flag0;
  int flag1;
  int flag2;
  int flag3;
  int flag4;
  int flag5;
  int flag6;
  int flag7;
  int flag8;
  float voltage1;          // 27.0
  int flag9;               
  int flag10;
  float voltage2;          // 23.0
  int currentSetting;      // 10
  float voltage3;          // 22.0
};

QPIRI_Data qpiri;

String qpiriStr = "(230.0 18.2 230.0 50.0 18.2 4200 4200 24.0 23.0 20.0 28.2 27.0 0 030 060 0 1 0 1 01 0 0 27.0 0 1 23.0 10 22.0\"w)";

void parseQPIRI(String response) {
  // ลบวงเล็บ และ "w" ตัวสุดท้าย
  response.replace("(", "");
  response.replace(")", "");
  response.replace("\"w", "");

  char dataArray[response.length() + 1];
  response.toCharArray(dataArray, sizeof(dataArray));

  char *token = strtok(dataArray, " ");
  int index = 0;

  while (token != NULL) {
    switch (index) {
      case 0: qpiri.acVoltage = atof(token); break;
      case 1: qpiri.acCurrent = atof(token); break;
      case 2: qpiri.acVoltage2 = atof(token); break;
      case 3: qpiri.acFrequency = atof(token); break;
      case 4: qpiri.acCurrent2 = atof(token); break;
      case 5: qpiri.ratedPower = strtoul(token, NULL, 10); break;
      case 6: qpiri.maxPower = strtoul(token, NULL, 10); break;
      case 7: qpiri.busVoltage = atof(token); break;
      case 8: qpiri.batteryVoltage = atof(token); break;
      case 9: qpiri.lowBatteryCutoff = atof(token); break;
      case 10: qpiri.maxChargeVoltage = atof(token); break;
      case 11: qpiri.floatVoltage = atof(token); break;
      case 12: qpiri.flag0 = atoi(token); break;
      case 13: qpiri.flag1 = atoi(token); break;
      case 14: qpiri.flag2 = atoi(token); break;
      case 15: qpiri.flag3 = atoi(token); break;
      case 16: qpiri.flag4 = atoi(token); break;
      case 17: qpiri.flag5 = atoi(token); break;
      case 18: qpiri.flag6 = atoi(token); break;
      case 19: qpiri.flag7 = atoi(token); break;
      case 20: qpiri.flag8 = atoi(token); break;
      case 21: qpiri.voltage1 = atof(token); break;
      case 22: qpiri.flag9 = atoi(token); break;
      case 23: qpiri.flag10 = atoi(token); break;
      case 24: qpiri.voltage2 = atof(token); break;
      case 25: qpiri.currentSetting = atoi(token); break;
      case 26: qpiri.voltage3 = atof(token); break;
      default: break;
    }
    token = strtok(NULL, " ");
    index++;
  }
}

void setup() {
  Serial.begin(115200);

  parseQPIRI(qpiriStr);

  Serial.println("=== QPIRI Parsed Data ===");
  Serial.println("AC Voltage: " + String(qpiri.acVoltage));
  Serial.println("AC Current: " + String(qpiri.acCurrent));
  Serial.println("AC Frequency: " + String(qpiri.acFrequency));
  Serial.println("Rated Power: " + String(qpiri.ratedPower));
  Serial.println("Battery Voltage: " + String(qpiri.batteryVoltage));
  Serial.println("Float Voltage: " + String(qpiri.floatVoltage));
  Serial.println("Voltage1: " + String(qpiri.voltage1));
  Serial.println("Voltage2: " + String(qpiri.voltage2));
  Serial.println("Voltage3: " + String(qpiri.voltage3));
}

void loop() {
  // เรียกใช้ค่าจาก qpiri struct ได้เลย
}
