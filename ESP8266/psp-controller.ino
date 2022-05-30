#include <ESP8266WiFi.h>

#define LED 2

char const *ssid = "psp-esp8266";
char const *pass = "";
WiFiServer server(80); // port 80 for web server
WiFiClient client;
String header = "";

int new_device     = 0;
int current_device = 0;

void setup() {
  Serial.begin(115200);

  // Setup ap mode
  Serial.println("Setting up AP Mode...");
  Serial.println("");

  if (WiFi.softAP(ssid, pass)) {
    Serial.print("Access Point is created with SSID: ");
    Serial.println(ssid);
    Serial.println("");
    Serial.print("Access Point IP: ");
    Serial.print(WiFi.softAPIP());

    delay(100);

    Serial.println();
    Serial.println("Web server started!");
    server.begin();
    server.setNoDelay(true);

  } else {
    Serial.println("Err: Access Point Failed to create...");
    return;
  }

  pinMode(LED, OUTPUT);
}

void loop() {
  //checkStationCount();
  main_code();
}

void main_code() {
  // get client request
  client = server.available();

  // check the client if connected
  while (client.connected()) {
    // get data client's request
    if (client.available()) {
      header = client.readStringUntil('\r');

      if (header.indexOf("/LEDOff") >= 0) {
        digitalWrite(LED, HIGH);
        Serial.println("OFF");
      } else if (header.indexOf("/LEDOn") >= 0) {
        digitalWrite(LED, LOW);
        Serial.println("ON");
      }
    }
  }
}

// Check how many device connected

void checkStationCount() {
  new_device = WiFi.softAPgetStationNum();
  if (current_device == new_device) {
    return;
  }

  if (current_device < new_device) {
    current_device = new_device;
    Serial.print("New Device Connected! - Total Device/s: ");
    Serial.println(current_device);
  }

  if (current_device > new_device) {
    current_device = new_device;
    Serial.print("Device Disconnected! - Total Device/s: ");
    Serial.println(current_device);
  }
}