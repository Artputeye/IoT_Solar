#include "invCommand.h"
// ************************  invCommand class  ************************
// public:

void invCommand::serialSent()
{
  while (Serial.available() > 0)
  {
    serialData = Serial.readStringUntil('\n');
    Serial.print("Serial Sent : ");
    Serial.println(serialData);
    cmd_inv(serialData);
    len = serialData.length();
    Serial.println("len1: " + String(len));
    delay(10);
  }
}

void invCommand::Response()
{
  unsigned long startTime = millis();
  const unsigned long timeout = 500; // อยู่ใน loop ไม่เกิน 500ms

  while (Serial2.available() > 0 && millis() - startTime < timeout)
  {
    invData = Serial2.readStringUntil('\n');
    Serial.println("Inverter respond");
    Serial.println(invData);
    len = invData.length();
    Serial.println("len: " + String(len));

    if (len == 110) parseQPIGS(invData);
    if (len == 112) parseQPIRI(invData);
    if (len == 36)  parseQPIWS(invData);

    vTaskDelay(10);
  }
}

uint16_t invCommand::modbusCRC(const uint8_t *buf, uint16_t len)
{
  uint16_t crc = 0xFFFF;
  for (uint16_t pos = 0; pos < len; pos++)
  {
    crc ^= (uint16_t)buf[pos];
    for (uint8_t i = 0; i < 8; i++)
    {
      if (crc & 0x0001)
      {
        crc >>= 1;
        crc ^= 0xA001;
      }
      else
      {
        crc >>= 1;
      }
    }
  }
  return crc;
}

// ฟังก์ชันสร้าง Modbus Write Single Register (0x06)
size_t invCommand::buildModbusWrite(uint8_t slaveID, uint16_t regAddr, uint16_t value, uint8_t *frame)
{
  frame[0] = slaveID;        // Slave ID
  frame[1] = 0x06;           // Function code (Write Single Register)
  frame[2] = regAddr >> 8;   // Register High
  frame[3] = regAddr & 0xFF; // Register Low
  frame[4] = value >> 8;     // Value High
  frame[5] = value & 0xFF;   // Value Low

  uint16_t crc = modbusCRC(frame, 6);
  frame[6] = crc & 0xFF; // CRC Low
  frame[7] = crc >> 8;   // CRC High

  return 8; // frame length
}

void invCommand::valueToinv(String Name, uint16_t val)
{
  uint8_t frame[8];
  auto it = InvAddress.find(Name); // ตรวจสอบชื่อ register
  if (it == InvAddress.end())
  {
    Serial.println("Register not found: " + Name);
    return;
  }

  uint16_t regAddr = it->second;                            // ดึงค่าจริงจาก map
  size_t len = buildModbusWrite(0x05, regAddr, val, frame); // สร้าง Modbus frame
  Serial2.write(frame, len);                                // ส่งคำสั่งไปยัง Inverter ผ่าน Serial2 ตามจำนวนไบต์จริง

  Serial.print("Command HEX: "); // แสดง HEX frame ใน Serial
  for (size_t i = 0; i < len; i++)
  {
    if (frame[i] < 0x10)
      Serial.print("0");
    Serial.print(frame[i], HEX);
    Serial.print(" ");
  }
  Serial.println();
}

void invCommand::cmd_inv(String data)
{
  // inquiry command to inverter
  // it will be calculated and added before send) // crc "\xB7\xA9" // CR "\x0D"
  byte QPIGS[] = {0x51, 0x50, 0x49, 0x47, 0x53, 0xB7, 0xA9, 0x0D}; // len = 110 Device general status parameters inquiry (230.8 49.9 230.8 49.9 0830 0617 019 360 08.6  0 0 000 0032 00.0042.  00.00 00000 00010000 00 00 000 1 010�
  byte QPIRI[] = {0X51, 0X50, 0X49, 0X52, 0X49, 0XF8, 0X54, 0x0D}; // len = 112 Device Rating Information inquiry (230.0 18.2 230.0 50.0 18.2 42 0 4200 24.0 23.0 20.028.2 27.0 0 002 060 0 1 1 1 01 0 0 29.0 0 1  3.0 10 22.0Y;
  byte QFLAG[] = {0X51, 0X46, 0X4C, 0X41, 0X47, 0X98, 0X74, 0x0D}; // len = 18 Device flag status inquiry (EbuxzglDajkvyd�
  byte QPIWS[] = {0X51, 0X50, 0X49, 0X57, 0X53, 0xB4, 0XDA, 0x0D}; // len = 36 DeviceWarning Status inquiry (000000000000001000000010000000 0t�
  byte QDI[] = {0X51, 0X44, 0X49, 0x71, 0X1B, 0x0D};               // len = 85 The default setting value information (230.0  0.0 0030 21.0 27.0 28.223.0 50 0 0 2 0 0 00 0 1 1 1 0 1 0 27.  0 1 0 1 0Dk
  byte QMOD[] = {0X51, 0X4D, 0X4F, 0X44, 0X49, 0XC1, 0x0D};        // len = 5 Device Mode inquiry (L�
  ////////////////////////////////////////////////////////////////////////

  // Inquiry to inverter
  if (data == "QPIGS") // Device general status parameters inquiry
  {
    Serial2.write(QPIGS, sizeof(QPIGS));
    if (print)
    {
      sentinv(data);
    }
  }

  if (data == "QPIRI") // Device Rating Information inquiry
  {
    Serial2.write(QPIRI, sizeof(QPIRI));
    if (print)
    {
      sentinv(data);
    }
  }

  if (data == "QFLAG") // Device flag status inquiry
  {
    Serial2.write(QFLAG, sizeof(QPIRI));
    if (print)
    {
      sentinv(data);
    }
  }

  if (data == "QPIWS") // DeviceWarning Status inquiry
  {
    Serial2.write(QPIWS, sizeof(QPIRI));
    if (print)
    {
      sentinv(data);
    }
  }

  if (data == "QDI") // The default setting value information
  {
    Serial2.write(QDI, sizeof(QPIRI));
    if (print)
    {
      sentinv(data);
    }
  }

  if (data == "QMOD") // The default setting value information
  {
    Serial2.write(QMOD, sizeof(QPIRI));
    if (print)
    {
      sentinv(data);
    }
  }

  /////////////////////////////////////////////////////////////////////////////////
  // ESP Reset

  if (data == "espreset")
  {
    Serial.println("ESP Reset");
    delay(5000);
    ESP.restart();
  }

  ////////////////////////////////////////////////////////////////////////////////
  // Run mode
  if (data == "run mode")
  {
    RunMode = true;
    Serial.println("Run mode");
  }
  if (data == "stop mode")
  {
    RunMode = false;
    Serial.println("Stop mode");
  }

  ////////////////////////////////////////////////////////////////////////////////
  // wifi configuration
  if (data == "wifi mode 1")
  {
    wifi_config = true;
    Serial.println("wifi mode 1");
  }
  if (data == "wifi mode 0")
  {
    wifi_config = false;
    Serial.println("wifi mode 0");
  }

  ////////////////////////////////////////////////////////////////////////////////
  // ip configuration
  if (data == "ip mode 1")
  {
    ip_config = true;
    Serial.println("ip mode 1");
  }
  if (data == "ip mode 0")
  {
    ip_config = false;
    Serial.println("ip mode 0");
  }

  ////////////////////////////////////////////////////////////////////////////////
  // debug mode

  if (data == "debug1 on")
  {
    debug1 = true;
    Serial.println("Debug1 mode on");
  }
  if (data == "debug1 off")
  {
    debug1 = false;
    Serial.println("Debug1 mode off ");
  }

  ////////////////////////////////////////////////////////////////////////////////
  // Test mode

  if (data == "test on")
  {
    test = true;
    Serial.println("test mode on");
  }
  if (data == "test off")
  {
    test = false;
    Serial.println("test mode off");
  }

  ////////////////////////////////////////////////////////////////////////////////
  // print mode

  if (data == "print on")
  {
    print = true;
    Serial.println("print mode on");
  }
  if (data == "print off")
  {
    print = false;
    Serial.println("print mode off");
  }

  /////////////////////////////////////////////////////////////////////////////////
  // reset wifi para

  if (data == "para res")
  {
    para = true;
    Serial.println("Resset setting");
  }

  /////////////////////////////////////////////////////////////////////////////////
  // read DIR SPIFFS

  if (data == "littleFS")
  {
    dir = true;
    Serial.println("read DIR SPIFFS");
  }

  /////////////////////////////////////////////////////////////////////////////////
  // Format SPIFFS

  if (data == "formatFS")
  {
    format = true;
    Serial.println("Format SPIFFS");
  }

  /////////////////////////////////////////////////////////////////////////////////
  // Help

  if (data == "help")
  {
    help();
  }
}

// private:
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////

void invCommand::parseQPIGS(String response)
{
  if (response.startsWith("("))
  {
    response = response.substring(1, response.length() - 1); // Remove parentheses
    char dataArray[response.length() + 1];
    response.toCharArray(dataArray, sizeof(dataArray)); // Convert to C-string
    char *token = strtok(dataArray, " ");               // Split by space // First token
    int index = 0;
    while (token != NULL)
    {
      switch (index)
      {
      case 0:
        data.gridVoltage = atof(token);
        break; // Convert to float
      case 1:
        data.gridFrequency = atof(token);
        break;
      case 2:
        data.outputVoltage = atof(token);
        break;
      case 3:
        data.outputFrequency = atof(token);
        break;
      case 4:
        data.ApparentPower = strtoul(token, NULL, 10);
        break;
      case 5:
        data.ActivePower = strtoul(token, NULL, 10);
        break;
      case 6:
        data.loadPercent = strtoul(token, NULL, 10);
        break;
      case 7:
        data.busVoltage = strtoul(token, NULL, 10);
        break;
      case 8:
        data.batteryVoltage = atof(token);
        break;
      case 9:
        data.unknow9 = strtoul(token, NULL, 10);
        break;
      case 10:
        data.unknow10 = strtoul(token, NULL, 10);
        break;
      case 11:
        data.temp = strtoul(token, NULL, 10);
        break;
      case 12:
        data.pvCurrent = atof(token);
        break;
      case 13:
        data.pvVoltage = atof(token);
        break;
      case 14:
        data.unknow14 = atof(token);
        break;
      case 15:
        data.unknow15 = strtoul(token, NULL, 10);
        break;
      case 16:
        data.InverterStatus = strtoul(token, NULL, 10);
        break;
      case 17:
        data.unknow17 = strtoul(token, NULL, 10);
        break;
      case 18:
        data.unknow18 = strtoul(token, NULL, 10);
        break;
      case 19:
        data.unknow19 = strtoul(token, NULL, 10);
        break;
      case 20:
        data.unknow20 = strtoul(token, NULL, 10);
        break;
      }
      token = strtok(NULL, " ");
      index++;
    }
  }
}

void invCommand::parseQPIRI(String response)
{
  if (response.startsWith("("))
  {
    response = response.substring(1, response.length() - 1); // Remove parentheses
    char dataArray[response.length() + 1];
    response.toCharArray(dataArray, sizeof(dataArray)); // Convert to C-string
    char *token = strtok(dataArray, " ");               // Split by space // First token
    int index = 0;
    while (token != NULL)
    {
      switch (index)
      {
      case 0:
        rated.GridRatingVoltage = atof(token);
        break; // Convert to float
      case 1:
        rated.GridRatingCurrent = atof(token);
        break;
      case 2:
        rated.OutputRatingVoltage = atof(token);
        break;
      case 3:
        rated.OutputRatingFrequency = atof(token);
        break;
      case 4:
        rated.OutputRatingCurrent = strtoul(token, NULL, 10);
        break;
      case 5:
        rated.OutputRatingApparentPower = strtoul(token, NULL, 10);
        break;
      case 6:
        rated.OutputRatingActivePower = strtoul(token, NULL, 10);
        break;
      case 7:
        rated.BatteryRatingVoltage = strtoul(token, NULL, 10);
        break;
      case 8:
        rated.BatteryReChargeVoltage = atof(token);
        break;
      case 9:
        rated.BatteryUnderVoltage = strtoul(token, NULL, 10);
        break;
      case 10:
        rated.BatteryBulkVoltage = strtoul(token, NULL, 10);
        break;
      case 11:
        rated.BatteryFloatVoltage = strtoul(token, NULL, 10);
        break;
      case 12:
        rated.BatteryType = atof(token);
        break;
      case 13:
        rated.MaxAC_ChargingCurrent = atof(token);
        break;
      case 14:
        rated.MaxChargingCurrent = atof(token);
        break;
      case 15:
        rated.InputVoltageRange = strtoul(token, NULL, 10);
        break;
      case 16:
        rated.OutputSourcePriority = strtoul(token, NULL, 10);
        break;
      case 17:
        rated.ChargerSourcePriority = strtoul(token, NULL, 10);
        break;
      case 18:
        rated.ParallelMaxNum = strtoul(token, NULL, 10);
        break;
      case 19:
        rated.MachineType = strtoul(token, NULL, 10);
        break;
      case 20:
        rated.Topology = strtoul(token, NULL, 10);
        break;
      case 21:
        rated.OutputMode = strtoul(token, NULL, 10);
        break;
      case 22:
        rated.BatteryReDischargeVoltage = strtoul(token, NULL, 10);
        break;
      case 23:
        rated.PV_Parallel = strtoul(token, NULL, 10);
        break;
      case 24:
        rated.PV_Balance = strtoul(token, NULL, 10);
        break;
      case 25:
        rated.MaxChargingTime = strtoul(token, NULL, 10);
        break;
      case 26:
        rated.OperationLogic = strtoul(token, NULL, 10);
        break;
      case 27:
        rated.MaxDischargingCurrent = strtoul(token, NULL, 10);
        break;
      }
      token = strtok(NULL, " ");
      index++;
    }
  }
}

void invCommand::parseQPIWS(const String &resp)
{
  if (resp.length() < 34)
  {
    faultList = "Invalid QPIWS";
    Serial.println("QPIWS too short: " + resp);
    Serial.println("faultList: " + faultList);
    return;
  }
  // ดึงเฉพาะ 32 บิต (index 1 ถึง 32)
  String bits = resp.substring(1, 33);
  faultList = "";
  for (int i = 0; i < 32; i++)
  {
    if (bits[i] == '1' && strncmp(faultNames[i], "reserved", 8) != 0)
    {
      if (faultList.length())
        faultList += ", ";
      faultList += faultNames[i];
    }
  }
  if (!faultList.length())
  {
    faultList = "Normal";
  }
  // Serial print เพื่อตรวจสอบ
  Serial.println("QPIWS Response: " + resp);
  Serial.println("Parsed Fault List: " + faultList);
}

void invCommand::sentinv(String data)
{
  Serial.println("Sent command " + data + " to Inverter");
}

void invCommand::help()
{
  Serial.println("*********************** command ***********************");
  Serial.println("QPIGS //Device general status parameters inquiry");
  Serial.println("QPIRI //Device Rating Information inquiry");
  Serial.println("QFLAG //Device flag status inquiry");
  Serial.println("QPIWS //DeviceWarning Status inquiry");
  Serial.println("QDI //The default setting value information");
  Serial.println("QMOD //Device Mode inquiry");
  Serial.println("run mode //Sent QPIGS to inverter any 3 second");
  Serial.println("stop mode //Stop sent QPIGS to inverter any 3 second");
  Serial.println("wifi mode 1 //Access Point Mode");
  Serial.println("wifi mode 0 //Station Moe");
  Serial.println("ip mode 1 //Staic IP");
  Serial.println("ip mode 0 //DHCP");
  Serial.println("print on");
  Serial.println("print off");
  Serial.println("test on");
  Serial.println("test off");
  Serial.println("para res //reset setting");
  Serial.println("littleFS //read DIR SPIFFS");
  Serial.println("formatFS //Format SPIFFS");
}
