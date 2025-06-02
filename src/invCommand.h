#ifndef INV_COMMAND_H
#define INV_COMMAND_H

#include <Arduino.h>
#include <string.h>

#define RX_pin 16
#define TX_pin 17

class invCommand
{
public:
  /***************************************** public variable********************************/
  String invData;
  String inputString;

  unsigned int len;
  bool test;
  bool print;
  bool para;
  bool format;

  // Structure to store the data for QPIGS
  struct Vals_t
  {
    float gridVoltage;       // 0
    float gridFrequency;     // 1
    float outputVoltage;     // 2
    float outputFrequency;   // 3
    uint32_t ApparentPower;  // 4
    uint32_t ActivePower;    // 5
    uint32_t loadPercent;    // 6
    uint32_t busVoltage;     // 7
    float batteryVoltage;    // 8
    uint32_t unknow9;        // 9
    uint32_t unknow10;       // 10
    uint32_t temp;           // 11
    float pvCurrent;         // 12
    float pvVoltage;         // 13
    float unknow14;          // 14
    uint32_t unknow15;       // 15
    uint32_t InverterStatus; // 16
    uint32_t unknow17;       // 17
    uint32_t unknow18;       // 18
    uint32_t unknow19;       // 19
    uint32_t unknow20;       // 20
  } data;

  /**************************************** public function ********************************/
  void readData();
  void serialSent();
  void Response();
  void cmd_inv(String data);

private:
  /***************************************** private function *******************************/
  void qpigs_parse(String response);
  void qpigs_print();
  void parseInverterResponse(String response);
  void sentinv(String data);
  void help();
};

extern invCommand inv; // ประกาศ global object เพื่อใช้นอก cpp

#endif