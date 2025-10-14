#include "littleFS_data.h"
const char *targetDirectory = "/";
String energyFile = "/energy.json";
unsigned long lastSaveTime = 0;
unsigned long lastClearEnergy = 0;
const unsigned long saveInterval = 1UL * 60UL * 1000UL; // 15 นาที

void fileManage()
{
  if (inv.dir)
  {
    listAllFilesAndFolders(targetDirectory);
    inv.dir = false;
  }

  if (inv.format)
  {
    LittleFS.format();
    inv.format = false;
  }
}

void listAllFilesAndFolders(const char *dirname)
{
  // แสดงรายชื่อไฟล์ทั้งหมดใน LittleFS
  Serial.println("\nListing files in LittleFS:");
  Serial.printf("Listing directory: %s\n", dirname);

  File root = LittleFS.open(dirname);
  if (!root)
  {
    Serial.println("Failed to open directory");
    return;
  }
  if (!root.isDirectory())
  {
    Serial.println("Not a directory");
    return;
  }

  File file = root.openNextFile();
  while (file)
  {
    if (file.isDirectory())
    {
      Serial.print("  DIR : ");
      Serial.println(file.name());
      // ถ้าต้องการดูไฟล์ใน sub-directory ด้วย ให้เรียกซ้ำ:
      listAllFilesAndFolders(file.name()); // ระวัง recursive depth ถ้ามี subfolder เยอะ
    }
    else
    {
      Serial.print("  FILE: ");
      Serial.print(file.name());
      Serial.print("\tSIZE: ");
      Serial.print(file.size());
      Serial.println(" bytes");
    }
    file = root.openNextFile();
  }
}

// ======================================================
// 🔹 บันทึกค่า energy_kWh ลงใน LittleFS
// ======================================================
void saveEnergyToFile()
{
  JsonDocument doc;
  doc["energy_kWh"] = energy_kWh;

  File file = LittleFS.open(energyFile, "w");
  if (!file)
  {
    Serial.println("❌ Failed to open file for writing");
    return;
  }

  serializeJson(doc, file);
  file.close();

  Serial.printf("💾 Saved energy_kWh: %.4f kWh to %s\n", energy_kWh, energyFile.c_str());
}

// ======================================================
// 🔹 โหลดค่า energy_kWh จาก LittleFS (ตอนเริ่มต้น)
// ======================================================
void loadEnergyFromFile()
{
  if (!LittleFS.exists(energyFile))
  {
    Serial.println("⚠️ No previous energy file found");
    return;
  }
  File file = LittleFS.open(energyFile, "r");
  if (!file)
  {
    Serial.println("❌ Failed to open energy file for reading");
    return;
  }
  JsonDocument doc;
  DeserializationError error = deserializeJson(doc, file);
  if (error)
  {
    Serial.println("❌ Failed to parse energy.json");
    file.close();
    return;
  }
  energy_kWh = doc["energy_kWh"].as<float>();
  file.close();

  Serial.printf("✅ Loaded previous energy_kWh: %.4f kWh\n", energy_kWh);
}

// ======================================================
// 🔹 ลบไฟล์ energy.json (ใช้ตอน 18:00)
// ======================================================
void clearEnergyFile()
{
  if (LittleFS.exists(energyFile))
  {
    energy_kWh = 0.0;
    saveEnergyToFile();
    Serial.println("🧹 Cleared energy.json file");
  }
}

// ======================================================
// 🔹 ตรวจเวลาทุก loop — บันทึก / ลบไฟล์ / โหลดเวลา
// ======================================================
void handleEnergyStorage()
{
  if (!getLocalTime(&timeinfo))
  {
    Serial.println("⚠️ Failed to obtain time");
    return;
  }
  unsigned long currentMillis = millis();
  if (currentMillis - lastSaveTime >= saveInterval)
  {
    lastSaveTime = currentMillis;
    saveEnergyToFile();
  }

}