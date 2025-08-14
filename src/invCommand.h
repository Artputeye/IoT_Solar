#ifndef INV_COMMAND_H
#define INV_COMMAND_H
#include <Arduino.h>
#include <string.h>

class invCommand
{
public:
  /***************************************** public variable********************************/
  String invData;
  String inputString;
  unsigned int len;

  // parameter setting
  bool test = false;   // test sent data to home assistant
  bool print = false;  // serail print data
  bool para = false;   // parameter reste
  bool format = false; // fomat littleFS
  bool dir = false;    // dir littleFS
  bool RunMode = true; // auto mode

  // Structure to store the data for QPIGS
  struct QPIGSVals_t
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

  struct QPIRIvals_t // Device Rating Information inquiry
  {
    uint32_t _unixtime;                   // When the strucure was read from inverter (epoch in seconds)
    uint32_t GridRatingVoltage;           // Grid rating voltage               xx.x V * 10
    uint32_t GridRatingCurrent;           // Grid rating current               xx.x A * 10
    uint32_t ACOutputRatingVoltage;       // AC output rating voltage          xxx.x V * 10
    uint32_t ACOutputRatingFrequency;     // AC output rating frequency        xx.x Hz * 10
    uint32_t ACOutputRatingCurrent;       // AC output rating current          xx.x A * 10
    uint32_t ACOutputRatingApparentPower; // AC output rating apparent power   xxxx VA
    uint32_t ACOutputRatingActivePower;   // AC output rating active power     xxxx W
    uint32_t BatteryRatingVoltage;        // Battery rating voltage            xx.x V * 10
    uint32_t BatteryReChargeVoltage;      // Battery re-charge voltage         xx.x V * 10
    uint32_t BatteryUnderVoltage;         // Battery under voltage             xx.x V * 10
    uint32_t BatteryBulkVoltage;          // Battery bulk voltage              xx.x V * 10
    uint32_t BatteryFloatVoltage;         // Battery float voltage             xx.x V * 10
    uint8_t BatteryType;                  // Battery type 0-9                  0: AGM 1: Flooded 2: User 3: Pylon 5: Weco 6: Soltaro 8: Lib 9: Lic (5,6,8,9 protocol 2 )
    uint8_t MaxAC_ChargingCurrent;        // Max AC charging current           xxx A
    uint8_t MaxChargingCurrent;           // Max charging current              xxx A
    uint8_t InputVoltageRange;            // input voltage range 0-1           0: Appliance 1: UPS
    uint8_t OutputSourcePriority;         // output source priority 0-2        0: UtilitySolarBat 1: SolarUtilityBat 2: SolarBatUtility
    uint8_t ChargerSourcePriority;        // charger source priority 0-3       0: Utility first 1: Solar first 2: Solar + Utility 3: Only solar charging permitted ( protocol 2 1-3 )
    uint8_t ParallelMaxNum;               // parallel max num 0-9
    uint8_t MachineType;                  // Machine type                      00: Grid tie; 01: Off Grid; 10: Hybrid.
    uint8_t Topology;                     // Topology                          0: transformerless 1: transformer
    uint8_t OutputMode;                   // Output mode 0-7
                                          // 00: single machine output
                                          // 01: parallel output 02: Phase 1 of 3 Phase output
                                          // 03: Phase 2 of 3 Phase output
                                          // 04: Phase 3 of 3 Phase output
                                          // 05: Phase 1 of 2 Phase output
                                          // 06: Phase 2 of 2 Phase output (120°)
                                          // 07: Phase 2 of 2 Phase output (180°)
    uint32_t BatteryReDischargeVoltage;   // Battery re-discharge voltage xx.x V *10
    uint8_t PV_OK_ConditionParallel;      // PV OK condition for parallel
                                          // 0: As long as one unit of inverters has connect PV, parallel system willconsider PV OK;
                                          // 1: Only All of inverters have connect PV, parallel system will consider PV OK
    uint8_t PV_PowerBalance;              // PV power balance
                                          // 0: PV input max current will be the max charged current;
                                          // 1: PV input max power will be the sum of the max charged power and loads power.
    uint32_t MaxChargingTimeAtC_V_Stage;  // Max. charging time at C.V stage (only 48V model)     xxx minute  ( Protocol 2 only )
    uint8_t OperationLogic;               // Operation Logic (only 48V model) 0-2                             ( Protocol 2 only )
                                          // 0: Automatically
                                          // 1: On-line mode
                                          // 2: ECO mode
    uint8_t MaxDischargingCurrent;        // Max discharging current (only 48V model)     xxx A
  } rated;

  /**************************************** public function ********************************/
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

#endif