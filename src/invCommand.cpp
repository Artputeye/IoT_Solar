#include "invCommand.h"

// ************************  invCommand class  ************************
// public:

void invCommand::serialSent()
{
  while (Serial.available() > 0)
  {
    inputString = Serial.readStringUntil('\n');
    Serial.print("Serial Sent : ");
    Serial.println(inputString);
    cmd_inv(inputString);
    len = inputString.length();
    // Serial.println("len1: " + String(len));
    if (len == 109)
    {
      qpigs_parse(inputString);
    }
  }
}

void invCommand::Response()
{
  while (Serial2.available() > 0)
  {
    invData = Serial2.readStringUntil('\n');
    Serial.println(invData);
    Serial.println("response");
    len = invData.length();
    Serial.println("len2: " + String(len));

    if (len == 110)
    {
      qpigs_parse(invData);
    }
  }
}

void invCommand::cmd_inv(String data)
{
  // inquiry command to inverter
  // it will be calculated and added before send) // crc "\xB7\xA9" // CR "\x0D"
  byte QPIGS[] = {0x51, 0x50, 0x49, 0x47, 0x53, 0xB7, 0xA9, 0x0D}; // Device general status parameters inquiry (230.8 49.9 230.8 49.9 0830 0617 019 360 08.6  0 0 000 0032 00.0042.  00.00 00000 00010000 00 00 000 1 010�
  byte QPIRI[] = {0X51, 0X50, 0X49, 0X52, 0X49, 0XF8, 0X54, 0x0D}; // Device Rating Information inquiry (230.0 18.2 230.0 50.0 18.2 42 0 4200 24.0 23.0 20.028.2 27.0 0 002 060 0 1 1 1 01 0 0 29.0 0 1  3.0 10 22.0Y;
  byte QFLAG[] = {0X51, 0X46, 0X4C, 0X41, 0X47, 0X98, 0X74, 0x0D}; // Device flag status inquiry (EbuxzglDajkvyd�
  byte QPIWS[] = {0X51, 0X50, 0X49, 0X57, 0X53, 0xB4, 0XDA, 0x0D}; // DeviceWarning Status inquiry (000000000000001000000010000000 0t�
  byte QDI[] = {0X51, 0X44, 0X49, 0x71, 0X1B, 0x0D};               // The default setting value information (230.0  0.0 0030 21.0 27.0 28.223.0 50 0 0 2 0 0 00 0 1 1 1 0 1 0 27.  0 1 0 1 0Dk
  byte QMOD[] = {0X51, 0X4D, 0X4F, 0X44, 0X49, 0XC1, 0x0D};        // Device Mode inquiry (L�
  ////////////////////////////////////////////////////////////////////////

  // Grid-tie operation
  byte grind_on[] = {0x05, 0x06, 0x13, 0xaa, 0x00, 0x01, 0x6d, 0x2a};
  byte grind_off[] = {0x05, 0x06, 0x13, 0xaa, 0x00, 0x00, 0xac, 0xea};

  // AC Input Range
  byte appliance[] = {0x05, 0x06, 0x13, 0x9b, 0x00, 0x00, 0xfd, 0x25}; //...›..ý% // Appliance
  byte ups[] = {0x05, 0x06, 0x13, 0x9b, 0x00, 0x01, 0x3c, 0xe5};       //...›..<å // UPS

  // Beeps While Primary Suorce Interrupt
  byte beeps_on[] = {0x05, 0x06, 0x13, 0x8f, 0x00, 0x01, 0x7c, 0xe1};        //.....|á // Enable
  byte beeps_off[] = {0x05, 0x06, 0x013, 0x08f, 0x000, 0x000, 0x0bd, 0x021}; //.....½! // Prohibited

  // LCD Blacklight
  byte blacklight_on[] = {0x05, 0x06, 0x13, 0x8c, 0x00, 0x01, 0x8c, 0xe1};  //...Œ..Œá // Enable
  byte blacklight_off[] = {0x05, 0x06, 0x13, 0x8c, 0x00, 0x00, 0x4d, 0x21}; //...Œ..M! // Prohibited

  // Return To The Main LCD Page
  byte returnpage_on[] = {0x05, 0x06, 0x13, 0x90, 0x00, 0x01, 0x4d, 0x27};  //.....M'  // Enable
  byte returnpage_off[] = {0x05, 0x06, 0x13, 0x90, 0x00, 0x00, 0x8c, 0xe7}; //.....Œç  // Prohibited

  // Power Saving Mode
  byte saving_on[] = {0x05, 0x06, 0x13, 0x8b, 0x00, 0x01, 0x3d, 0x20};  //...‹..=  // Enable
  byte saving_off[] = {0x05, 0x06, 0x13, 0x8b, 0x00, 0x00, 0xfc, 0xe0}; //...‹..üà // Prohibited

  // Over Temp Auto Restart
  byte overtemp_on[] = {0x05, 0x06, 0x13, 0x8e, 0x00, 0x01, 0x2d, 0x21};  //...Ž..-! // Enable
  byte overtemp_off[] = {0x05, 0x06, 0x13, 0x8e, 0x00, 0x00, 0xec, 0xe1}; //...Ž..ìá // Prohibited

  // led Pattern light
  byte ledpattern_on[] = {0x05, 0x06, 0x13, 0xac, 0x00, 0x01, 0x8d, 0x2b};  //...¬..+ // on
  byte ledpattern_off[] = {0x05, 0x06, 0x13, 0xac, 0x00, 0x00, 0x4c, 0xeb}; //...¬..Lë // off

  // Buzzer
  byte buzzer_on[] = {0x05, 0x06, 0x13, 0x8a, 0x00, 0x01, 0x6c, 0xe0};  //...Š..là // on
  byte buzzer_off[] = {0x05, 0x06, 0x13, 0x8a, 0x00, 0x00, 0xad, 0x20}; //...Š..­ // off

  // dual output
  byte dualoutput_on[] = {0x05, 0x06, 0x13, 0xad, 0x00, 0x01, 0xdc, 0xeb};  //...­..Üë // use
  byte dualoutput_off[] = {0x05, 0x06, 0x13, 0xad, 0x00, 0x00, 0x1d, 0x2b}; //...­...+ // disable

  // Overload Auto Restart
  byte overload_on[] = {0x05, 0x06, 0x13, 0x8d, 0x00, 0x01, 0xdd, 0x21};  //.....Ý! // Enable
  byte overload_off[] = {0x05, 0x06, 0x13, 0x8d, 0x00, 0x00, 0x1c, 0xe1}; //......á // Prohibited

  // Transfer To Bypass Overload
  byte bypass_on[] = {0x05, 0x06, 0x13, 0x91, 0x00, 0x01, 0x1c, 0xe7};  //...‘...ç  // Enable
  byte bypass_off[] = {0x05, 0x06, 0x13, 0x91, 0x00, 0x00, 0xdd, 0x27}; //...‘..Ý'  // Prohibited

  // Out Put Source Priority
  byte utility[] = {0x05, 0x06, 0x13, 0x9a, 0x00, 0x00, 0xac, 0xe5}; //...š..¬å // Utility
  byte solar[] = {0x05, 0x06, 0x13, 0x9a, 0x00, 0x01, 0x6d, 0x25};   //...š..m% // Solar
  byte sbu[] = {0x05, 0x06, 0x13, 0x9a, 0x00, 0x02, 0x2d, 0x24};     //...š..-$  // SBU

  // Output Voltage
  byte output_220[] = {0x05, 0x06, 0x13, 0x9f, 0x00, 0xdc, 0xbd, 0x7d}; //...Ÿ.Ü½} // 220Vac
  byte output_230[] = {0x05, 0x06, 0x13, 0x9f, 0x00, 0xe6, 0x3d, 0x6e}; //...Ÿ.æ=n // 230Vac
  byte output_240[] = {0x05, 0x06, 0x13, 0x9f, 0x00, 0xf0, 0xbc, 0xa0}; //...Ÿ.ð¼  // 240Vac

  // grid-tie current
  // 2A
  // 05 06 13 ab 00 01 3c ea                           //...«..<ê
  // 4A
  // 05 06 13 ab 00 02 7c eb                           //...«..|ë
  // 6A
  // 05 06 13 ab 00 03 bd 2b                           //...«..½+
  // 8A
  // 05 06 13 ab 00 04 fc e9                           //...«..üé
  // 10A
  // 05 06 13 ab 00 05 3d 29                           //...«..=)
  // 12A
  // 05 06 13 ab 00 06 7d 28                           //...«..}(
  // 14A
  // 05 06 13 ab 00 07 bc e8                           //...«..¼è
  // 16A
  // 05 06 13 ab 00 08 fc ec                           //...«..üì
  // 18A
  // 05 06 13 ab 00 09 3d 2c                           //...«..=,
  // 20A
  // 05 06 13 ab 00 0a 7d 2d                           //...«..}-
  // 22A
  // 05 06 13 ab 00 0b bc ed                           //...«..¼í
  // 24A
  // 05 06 13 ab 00 0c fd 2f                           //...«..ý/
  // 26A
  // 05 06 13 ab 00 0d 3c ef                           //...«..<ï
  // 28A
  // 05 06 13 ab 00 0e 7c ee                           //...«..|î
  // 30A
  // 05 06 13 ab 00 0f bd 2e                           //...«..½.

  // parameter setting
  if (data == "QPIGS")
  {
    Serial2.write(QPIGS, sizeof(QPIGS));
    if (print)
    {
      sentinv(data);
    }
  }

  /////////////////////////////////////////////////////////////////////////////////////////
  // ON/OFF Grind command to inverte
  if (data == "grid on" || data == "Grid-tie operation ON")
  {
    Serial2.write(grind_on, sizeof(grind_on));
    sentinv(data);
  }
  if (data == "grid off" || data == "Grid-tie operation OFF")
  {
    Serial2.write(grind_off, sizeof(grind_off));
    sentinv(data);
  }
  if (data == "appliance")
  {
    Serial2.write(appliance, sizeof(appliance));
    sentinv(data);
  }
  if (data == "ups")
  {
    Serial2.write(ups, sizeof(ups));
    sentinv(data);
  }
  if (data == "beeps on" || data == "Beeps While Primary Source Interrupt ON")
  {
    Serial2.write(beeps_on, sizeof(beeps_on));
    sentinv(data);
  }
  if (data == "beeps off" || data == "Beeps While Primary Source Interrupt OFF")
  {
    Serial2.write(beeps_off, sizeof(beeps_off));
    sentinv(data);
  }
  if (data == "blacklight on" || data == "LCD Backlight ON")
  {
    Serial2.write(blacklight_on, sizeof(blacklight_on));
    sentinv(data);
  }
  if (data == "blacklight off" || data == "LCD Backlight OFF")
  {
    Serial2.write(blacklight_off, sizeof(blacklight_off));
    sentinv(data);
  }
  if (data == "returnpage on" || data == "Return To The Main LCD Page ON")
  {
    Serial2.write(ledpattern_on, sizeof(ledpattern_on));
    sentinv(data);
  }
  if (data == "returnpage off" || data == "Return To The Main LCD Page OFF")
  {
    Serial2.write(ledpattern_off, sizeof(ledpattern_off));
    sentinv(data);
  }
  if (data == "saving on" || data == "Power Saving Mode ON")
  {
    Serial2.write(saving_on, sizeof(saving_on));
    sentinv(data);
  }
  if (data == "saving off" || data == "Power Saving Mode OFF")
  {
    Serial2.write(saving_off, sizeof(saving_off));
    sentinv(data);
  }
  if (data == "overtemp on" || data == "Over Temp Auto Restart ON")
  {
    Serial2.write(overtemp_on, sizeof(overtemp_on));
    sentinv(data);
  }
  if (data == "overtemp off" || data == "Over Temp Auto Restart OFF")
  {
    Serial2.write(overtemp_off, sizeof(overtemp_off));
    sentinv(data);
  }
  if (data == "ledpattern on" || data == "Led Pattern Light ON")
  {
    Serial2.write(ledpattern_on, sizeof(ledpattern_on));
    sentinv(data);
  }
  if (data == "ledpattern off" || data == "Led Pattern Light ON")
  {
    Serial2.write(ledpattern_off, sizeof(ledpattern_off));
    sentinv(data);
  }
  if (data == "buzzer on" || data == "Buzzer ON")
  {
    Serial2.write(buzzer_on, sizeof(buzzer_on));
    sentinv(data);
  }
  if (data == "buzzer off" || data == "Buzzer OFF")
  {
    Serial2.write(buzzer_off, sizeof(buzzer_off));
    sentinv(data);
  }
  if (data == "dualoutput on" || data == "Dual Output ON")
  {
    Serial2.write(dualoutput_on, sizeof(dualoutput_on));
    sentinv(data);
  }
  if (data == "dualoutput off" || data == "Dual Output OFF")
  {
    Serial2.write(dualoutput_off, sizeof(dualoutput_off));
    sentinv(data);
  }
  if (data == "overload on" || data == "Overload Auto Restart ON")
  {
    Serial2.write(overload_on, sizeof(overload_on));
    sentinv(data);
  }
  if (data == "overload off" || data == "Overload Auto Restart OFF")
  {
    Serial2.write(overload_off, sizeof(overload_off));
    sentinv(data);
  }
  if (data == "bypass on" || data == "Transfer To Bypass Overload ON")
  {
    Serial2.write(bypass_on, sizeof(bypass_on));
    sentinv(data);
  }
  if (data == "bypass off" || data == "Transfer To Bypass Overload OFF")
  {
    Serial2.write(bypass_off, sizeof(bypass_off));
    sentinv(data);
  }
  if (data == "utility")
  {
    Serial2.write(utility, sizeof(utility));
    sentinv(data);
  }
  if (data == "solar")
  {
    Serial2.write(solar, sizeof(solar));
    sentinv(data);
  }
  if (data == "sbu")
  {
    Serial2.write(sbu, sizeof(sbu));
    sentinv(data);
  }
  if (data == "output_220")
  {
    Serial2.write(output_220, sizeof(output_220));
    sentinv(data);
  }
  if (data == "output_230")
  {
    Serial2.write(output_230, sizeof(output_230));
    sentinv(data);
  }
  if (data == "output_240")
  {
    Serial2.write(output_240, sizeof(output_240));
    sentinv(data);
  }

  ////////////////////////////////////////////////////////////////////////////////
  // Test mode
  if (data == "test on")
  {
    test = true;
    Serial.println("test on mode");
  }
  if (data == "test off")
  {
    test = false;
    Serial.println("test off mode");
  }

  ////////////////////////////////////////////////////////////////////////////////
  // print mode
  if (data == "print on")
  {
    print = true;
    Serial.println("print on mode");
  }
  if (data == "print off")
  {
    print = false;
    Serial.println("print off mode");
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
void invCommand::qpigs_parse(String response)
{
  // QPIGS Response
  parseInverterResponse(response);
  qpigs_print();
}

void invCommand::qpigs_print()
{
  if (print)
  {
    Serial.print("Grid Voltage: "); // 0
    Serial.println(data.gridVoltage);
    Serial.print("Grid Frequency: "); // 1
    Serial.println(data.gridFrequency);
    Serial.print("Output Voltage: "); // 2
    Serial.println(data.outputVoltage);
    Serial.print("Output Frequency: "); // 3
    Serial.println(data.outputFrequency);
    Serial.print("Output ApparentPower: "); // 4
    Serial.println(data.ApparentPower);
    Serial.print("Output ActivePower: "); // 5
    Serial.println(data.ActivePower);
    Serial.print("Load Percent: "); // 6
    Serial.println(data.loadPercent);
    Serial.print("Bus Voltage: "); // 7
    Serial.println(data.busVoltage);
    Serial.print("Battery Voltage: "); // 8
    Serial.println(data.batteryVoltage);
    // Serial.print("unknow9: ");  //9
    // Serial.println(data.unknow9);
    // Serial.print("unknow10: ");  //10
    // Serial.println(data.unknow10);
    Serial.print("temp: "); // 11
    Serial.println(data.temp);
    Serial.print("PV Current: "); // 12
    Serial.println(data.pvCurrent);
    Serial.print("PV Voltage: "); // 13
    Serial.println(data.pvVoltage);
    Serial.print("unknow14: "); // 14
    Serial.println(data.unknow14);
    Serial.print("unknow15: "); // 15
    Serial.println(data.unknow15);
    Serial.print("Inverter Status: "); // 16
    Serial.println(data.InverterStatus);
    // Serial.print("unknow17: ");  //17
    // Serial.println(data.unknow17);
    // Serial.print("unknow18: ");  //18
    // Serial.println(data.unknow18);
    // Serial.print("unknow19: ");  //19
    // Serial.println(data.unknow19);
    // Serial.print("unknow20: ");  //20
    // Serial.println(data.unknow20);
  }
}

void invCommand::parseInverterResponse(String response)
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

void invCommand::sentinv(String data)
{
  Serial.println("Sent command " + data + " to Inverter");
}

void invCommand::help()
{
  Serial.println("*********************** command ***********************");
  Serial.println("QPIGS Device general status parameters inquiry");
  Serial.println("QPIRI Device Rating Information inquiry");
  Serial.println("QFLAG Device flag status inquiry");
  Serial.println("QPIWS DeviceWarning Status inquiry");
  Serial.println("QDI The default setting value information");
  Serial.println("QMOD Device Mode inquiry");
  Serial.println("grid on");
  Serial.println("grid off");
  Serial.println("print on");
  Serial.println("print off");
  Serial.println("test on");
  Serial.println("test off");
}
