#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

bool led_status = HIGH;
int led = D2;

/* Credentials */
const char *ssid = "psp-controller-server";
const char *pass = "";

ESP8266WebServer server(80); // port 80

void setup() {
  Serial.begin(115200);
  pinMode(led, OUTPUT);

  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, pass);
  Serial.println("Access Point Started");
  delay(200);
  server.on("/led_on", []() { led_status = HIGH; });
  server.on("/led_off", []() { led_status = LOW; });

  server.begin();
  Serial.println("Web server started\n");
  Serial.println("IP Address: ");
  Serial.println(WiFi.softAPIP()); // IP Address: 192.168.4.1
}

void loop() {
  server.handleClient();
  if (led_status)
    digitalWrite(led, HIGH);
  else
    digitalWrite(led, LOW);
}