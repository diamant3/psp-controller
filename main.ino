#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

// Set web server port number to 80
ESP8266WebServer server(80);

// Set BUILTIN LED
const int ledPin = BUILTIN_LED;

// Network credentials
const char *ssid = "PSP-CONTROLLER SERVER";
const char *password = "";

void setup() {
  // Initialize the LED pin as an output
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, HIGH);

  // Start Serial
  Serial.begin(115200);
  delay(10);

  // Configure as an access point
  Serial.println();
  Serial.println("Setting up WiFi AP...");
  WiFi.softAP(ssid, password);

  IPAddress IP = WiFi.softAPIP();

  // web server routes
  server.on("/on", []() { digitalWrite(ledPin, LOW); server.send(200, "text/html", ""); });
  server.on("/off", []() { digitalWrite(ledPin, HIGH); server.send(200, "text/html", ""); });

  // Start the server
  server.begin();
  Serial.print("HTTP server started: ");
  Serial.println(IP);
}

void loop() {
  // Handle client requests
  server.handleClient();
}
