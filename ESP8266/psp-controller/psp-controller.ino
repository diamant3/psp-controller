#include <ESP8266WiFi.h>

#define LED 2 // built-in led

// ap credentials
char const *ssid = "psp-esp8266";
char const *pass = "";

WiFiServer server(80); // server with port 80
WiFiClient client;     // client object

String header = "";

// device status
int new_device     = 0;
int current_device = 0;

void setup() 
{
  Serial.begin(115200);

  // Setup access point mode
  Serial.println("Setting up AP Mode...");
  Serial.println("");

  if (WiFi.softAP(ssid, pass)) 
  {
    Serial.print("Access Point is created with SSID: ");
    Serial.println(ssid);
    Serial.println("");
    Serial.print("Access Point IP: ");
    Serial.print(WiFi.softAPIP());
    Serial.println();
    Serial.println("Web server started!");
    server.begin();
    server.setNoDelay(true);
  } 
  else 
  {
    Serial.println("Err: Access Point Failed to create...");
  }

  pinMode(LED, OUTPUT);
}

void loop() 
{
  checkStationCount();
  main_code();
  delay(100);
}

void main_code() 
{
  // get client request
  client = server.available();

  // check the client if connected to esp8266
  while (client.connected()) 
  {
    // check if client is sending a data
    if (client.available()) 
    {
      // get the sent data
      header = client.readStringUntil('\r');

      if (header.indexOf("/LEDOff") >= 0) 
      {
        digitalWrite(LED, HIGH);
        Serial.println("OFF");
      } 
      
      if (header.indexOf("/LEDOn") >= 0) 
      {
        digitalWrite(LED, LOW);
        Serial.println("ON");
      }
    }
  }
}

// Check how many device connected
void checkStationCount() 
{
  new_device = WiFi.softAPgetStationNum();
  if (current_device == new_device) 
  {
    return;
  }

  if (current_device < new_device) 
  {
    current_device = new_device;
    Serial.print("New Device Connected! - Total Device/s: ");
    Serial.println(current_device);
  }

  if (current_device > new_device) 
  {
    current_device = new_device;
    Serial.print("Device Disconnected! - Total Device/s: ");
    Serial.println(current_device);
  }
}
