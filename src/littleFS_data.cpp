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
// 🔹 ฟังก์ชันโหลดค่า energy_kWh จาก LittleFS
// ======================================================
bool loadEnergyFromFile()
{
  if (!loadSetting())
  {
    Serial.println("⚠️ Using default gridOpr = 0 (manual mode)");
  }

  Serial.println("📂 Loading energy data...");

  if (!LittleFS.exists(energyFile))
  {
    Serial.println("⚠️ No previous energy.json found, setting energy_kWh = 0.0");
    energy_kWh = 0.0;
    return false;
  }

  File file = LittleFS.open(energyFile, "r");
  if (!file)
  {
    Serial.println("❌ Failed to open energy.json for reading");
    energy_kWh = 0.0;
    return false;
  }

  JsonDocument doc;
  DeserializationError error = deserializeJson(doc, file);
  file.close();

  if (error)
  {
    Serial.printf("❌ Failed to parse energy.json: %s\n", error.c_str());
    energy_kWh = 0.0;
    return false;
  }

  energy_kWh = doc["energy_kWh"] | 0.0;

  if (isnan(energy_kWh))
  {
    Serial.println("⚠️ Warning: Loaded value is NaN, resetting to 0.0");
    energy_kWh = 0.0;
    return false;
  }

  Serial.printf("✅ Loaded energy_kWh = %.4f kWh from %s\n", energy_kWh, energyFile.c_str());
  return true;
}

// ======================================================
// 🔹 ฟังก์ชันบันทึก energy_kWh ลงใน LittleFS
// ======================================================
bool saveEnergyToFile()
{
  Serial.printf("💾 Saving energy_kWh = %.4f to file...\n", energy_kWh);

  File file = LittleFS.open(energyFile, "w");
  if (!file)
  {
    Serial.println("❌ Failed to open energy.json for writing");
    return false;
  }

  JsonDocument doc;
  doc["energy_kWh"] = energy_kWh;

  size_t written = serializeJson(doc, file);
  file.flush();
  file.close();
  delay(50); // รอ Flash เขียนจริง

  if (written == 0)
  {
    Serial.println("❌ Failed to serialize JSON to file");
    return false;
  }

  // ตรวจสอบจากไฟล์จริงหลังบันทึก
  float verifyValue = 0.0;
  File verifyFile = LittleFS.open(energyFile, "r");
  if (verifyFile)
  {
    JsonDocument verifyDoc;
    if (deserializeJson(verifyDoc, verifyFile) == DeserializationError::Ok)
    {
      verifyValue = verifyDoc["energy_kWh"] | -1.0;
    }
    verifyFile.close();
  }

  if (fabs(verifyValue - energy_kWh) > 0.0001)
  {
    Serial.printf("⚠️ Warning: Mismatch detected (expected %.4f, got %.4f)\n", energy_kWh, verifyValue);
    return false;
  }

  Serial.printf("✅ File write verified: %.4f kWh saved successfully\n", energy_kWh);
  return true;
}

// ======================================================
// 🔹 ฟังก์ชันเคลียร์ค่า energy_kWh และรีเซ็ตไฟล์
// ======================================================
bool clearEnergyFile()
{
  Serial.println("🧹 Clearing energy.json file...");

  if (LittleFS.exists(energyFile))
  {
    if (!LittleFS.remove(energyFile))
    {
      Serial.println("❌ Failed to remove old energy.json");
      return false;
    }
    delay(50);
  }

  energy_kWh = 0.0;

  if (!saveEnergyToFile())
  {
    Serial.println("⚠️ Warning: Failed to create new cleared file, retrying...");
    delay(200);
    if (!saveEnergyToFile())
    {
      Serial.println("❌ Retry failed: energy_kWh reset failed");
      return false;
    }
  }

  if (!loadEnergyFromFile())
  {
    Serial.println("⚠️ Warning: Reload after clear failed");
    return false;
  }
  if (energy_kWh == 0.0)
  {
    Serial.println("✅ energy_kWh cleared successfully (0.0 kWh)");
    return true;
  }
  else
  {
    Serial.printf("⚠️ Warning: energy_kWh reset failed (%.4f != 0.0)\n", energy_kWh);
    return false;
  }
}

///////////////////////////////////////////////////////////////////////////////
bool loadJsonFile(const char *filename, JsonDocument &doc)
{
  Serial.printf("📂 Loading JSON from %s ...\n", filename);
  if (!LittleFS.exists(filename))
  {
    Serial.printf("⚠️ File not found: %s\n", filename);
    return false;
  }
  File file = LittleFS.open(filename, "r");
  if (!file)
  {
    Serial.printf("Failed to open %s for reading\n", filename);
    return false;
  }
  DeserializationError error = deserializeJson(doc, file);
  file.close();
  if (error)
  {
    Serial.printf("Failed to parse JSON (%s): %s\n",
                  filename, error.c_str());
    return false;
  }
  Serial.printf("JSON loaded successfully from %s\n", filename);
  return true;
}

///////////////////////////////////////////////////////////////////////////////
bool saveJsonFile(const char *filename, const JsonDocument &doc)
{
  Serial.printf("💾 Saving JSON to %s ...\n", filename);
  if (LittleFS.exists(filename))
  {
    LittleFS.remove(filename);
  }
  File file = LittleFS.open(filename, "w");
  if (!file)
  {
    Serial.printf("Failed to open %s for writing\n", filename);
    return false;
  }
  if (serializeJson(doc, file) == 0)
  {
    Serial.printf("Failed to write JSON to %s\n", filename);
    file.close();
    return false;
  }
  file.close();
  Serial.printf("JSON saved successfully to %s\n", filename);
  return true;
}

bool loadSetting()
{
    JsonDocument doc;
    if (loadJsonFile("/networkconfig.json", doc))
    {
        Serial.println("📂 Loaded networkconfig.json");
        wifimode = atoi(doc["wifi_mode"] | "0");
    }
    else
    {
        Serial.println("❌ Failed to load networkconfig.json");
        wifimode = 0;   // default
    }

    if (loadJsonFile("/setting.json", doc))
    {
        Serial.println("📂 Loaded setting.json");
        inv.gridOpr = atoi(doc["Grid Tie Auto"] | "0");
        gridCutOff = atoi(doc["gridCutOff"] | "");
        gridStart = atoi(doc["gridStart"] | "");
    }
    else
    {
        Serial.println("❌ Failed to load setting.json");
        inv.gridOpr = 0; // default
    }
   
    Serial.printf("🔧 Grid Settings Loaded:\n");
    Serial.printf("   - Wifi Mode         : %d\n", wifimode);
    Serial.printf("   - Grid Tie Auto     : %d\n", inv.gridOpr);
    Serial.printf("   - Grid Cut-Off Date : %d\n", gridCutOff);
    Serial.printf("   - Grid Start Date   : %d\n", gridStart);

    return true;
}

bool saveApSetting()
{
  JsonDocument doc;
  doc["wifi_mode"] = wifimode;
  if (saveJsonFile("/networkconfig.json", doc))
  {
    Serial.println("💾 setting.json saved successfully");
    return true;
  }
  else
  {
    Serial.println("❌ Failed to save setting.json");
    return false;
  }
}