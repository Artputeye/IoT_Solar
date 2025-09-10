#include "littleFS_data.h"
const char *targetDirectory = "/";

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