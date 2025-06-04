#include "globals.h"
#include "serv.h"
const char *PARAM_MESSAGE PROGMEM = "plain";

String getContentType(String filename)
{
  if (filename.endsWith(".html"))
    return "text/html";
  if (filename.endsWith(".css"))
    return "text/css";
  if (filename.endsWith(".js"))
    return "application/javascript";
  if (filename.endsWith(".png"))
    return "image/png";
  if (filename.endsWith(".jpg"))
    return "image/jpeg";
  if (filename.endsWith(".ico"))
    return "image/x-icon";
  if (filename.endsWith(".svg"))
    return "image/svg+xml";
  if (filename.endsWith(".json"))
    return "application/json";
  return "text/plain";
}

void setupServer()
{

  server.on("/post", HTTP_POST, [](AsyncWebServerRequest *request)
            {
    String message;
    if (request->hasParam(PARAM_MESSAGE, true)) {
      message = request->getParam(PARAM_MESSAGE, true)->value();
    } else {
      message = "No message sent";
      Serial.println("No message sent");
    }
    dataToinv(message);
    Serial.println("POST client: " + message);
    request->send(200, "text/plain", "POST: " + message); });

  // เสิร์ฟไฟล์ static จาก LittleFS
  server.onNotFound([](AsyncWebServerRequest *request)
                    {
    String path = request->url();
    //if (path == "/") path = "/index.html";

  if (!path.endsWith(".html") && !path.endsWith(".css") && !path.endsWith(".js")){
    if (path == "/") {
      path = "/index.html";
    } else {
      path += ".html"; // เช่น /about -> /about.html
    }
  }
    
    String contentType = getContentType(path);
    
    if (LittleFS.exists(path)) {
      request->send(LittleFS, path, contentType);
    } else {
      request->send(404, "text/plain", "File Not Found");
    } });

  server.begin();
}



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void dataToinv(String data)
{
  if (data == "Grid-tie operation ON")
  {
    inv.cmd_inv("grid on");
  }
  if (data == "Grid-tie operation OFF")
  {
    inv.cmd_inv("grid off");
  }
  if (data == "Beeps While Primary Source Interrupt ON")
  {
    inv.cmd_inv("beeps on");
  }
  if (data == "Beeps While Primary Source Interrupt OFF")
  {
    inv.cmd_inv("beeps off");
  }
  if (data == "LCD Backlight ON")
  {
    inv.cmd_inv("blacklight on");
  }
  if (data == "LCD Backlight OFF")
  {
    inv.cmd_inv("blacklight off");
  }
  if (data == "Return To The Main LCD Page ON")
  {
    inv.cmd_inv("returnpage on");
  }
  if (data == "Return To The Main LCD Page OFF")
  {
    inv.cmd_inv("returnpage off");
  }
  if (data == "Power Saving Mode ON")
  {
    inv.cmd_inv("saving on");
  }
  if (data == "Power Saving Mode OFF")
  {
    inv.cmd_inv("saving off");
  }
  if (data == "Over Temp Auto Restart ON")
  {
    inv.cmd_inv("overtemp on");
  }
  if (data == "Over Temp Auto Restart OFF")
  {
    inv.cmd_inv("overtemp off");
  }
  if (data == "Led Pattern Light ON")
  {
    inv.cmd_inv("ledpattern on");
  }
  if (data == "Led Pattern Light OFF")
  {
    inv.cmd_inv("ledpattern off");
  }
  if (data == "Buzzer ON")
  {
    inv.cmd_inv("buzzer on");
  }
  if (data == "Buzzer OFF")
  {
    inv.cmd_inv("buzzer off");
  }
  if (data == "Dual Output ON")
  {
    inv.cmd_inv("dualoutput on");
  }
  if (data == "Dual Output OFF")
  {
    inv.cmd_inv("dualoutput off");
  }
  if (data == "Overload Auto Restart ON")
  {
    inv.cmd_inv("dualoutput on");
  }
  if (data == "Overload Auto Restart OFF")
  {
    inv.cmd_inv("dualoutput off");
  }
  if (data == "Transfer To Bypass Overload ON")
  {
    inv.cmd_inv("dualoutput on");
  }
  if (data == "Transfer To Bypass Overload OFF")
  {
    inv.cmd_inv("dualoutput off");
  }
}