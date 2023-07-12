#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

bool led_status = HIGH;
const int led = D2;
const int port = 80;

/* Credentials */
const char *ssid = "psp-controller-server";
const char *pass = "";

ESP8266WebServer server(port);

void setup() {
  Serial.begin(115200);
  pinMode(led, OUTPUT);

  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, pass);
  delay(50);

  Serial.println("Access Point Started");
  server.on("/led_on", []() { led_status = HIGH; });
  server.on("/led_off", []() { led_status = LOW; });
  server.begin();
  delay(50);

  Serial.println("Server IP Address: ");
  Serial.println(WiFi.softAPIP()); // IP Address: 192.168.4.1
}

void loop() {
  server.handleClient();
  if (led_status) {
    digitalWrite(led, HIGH);
  } else {
    digitalWrite(led, LOW);
  }
  delay(1);
}
