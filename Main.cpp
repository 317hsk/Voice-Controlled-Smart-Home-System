#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

// WiFi Credentials
const char* ssid = "your wifi name";
const char* password = "your wifi password";

// GPIO Pins
#define RELAY1 5  // D1
#define RELAY2 4  // D2

// Relay states
bool relay1State = false;
bool relay2State = false;

// Web Server on Port 80
ESP8266WebServer server(80);

// Function to toggle relay
void toggleRelay(int relayPin, bool &state) {
  state = !state;
  digitalWrite(relayPin, state ? HIGH : LOW);
}

// Generate Web Interface
String generateHTML() {
  String html = "<!DOCTYPE html><html><head>";
  html += "<meta name='viewport' content='width=device-width, initial-scale=1.0'>";
  html += "<style>body{text-align:center;font-family:Arial;background-color:#f4f4f4;margin:50px;}";
  html += "button{padding:15px 30px;font-size:20px;margin:10px;border:none;border-radius:5px;cursor:pointer;}";
  html += ".on{background-color:Blue;color:white;}.off{background-color:blue;color:white;}";
  html += "</style></head><body>";
  html += "<h2>SMART HOME AUTOMATION SYSTEM</h2>";
  html += "<p>BULB 1: <button class='" + String(relay1State ? "on" : "off") + "' onclick='toggle(1)'>" + String(relay1State ? "ON" : "BULB1") + "</button></p>";
  html += "<p>BULB 2: <button class='" + String(relay2State ? "on" : "off") + "' onclick='toggle(2)'>" + String(relay2State ? "ON" : "BULB2") + "</button></p>";
  html += "<script>function toggle(relay){var x=new XMLHttpRequest();x.open('GET','/toggle'+relay,true);x.send();}</script>";
  html += "</body></html>";
  return html;
}

// Handle root page
void handleRoot() {
  server.send(200, "text/html", generateHTML());
}

// Toggle Relay 1
void handleToggle1() {
  toggleRelay(RELAY1, relay1State);
  handleRoot();
}

// Toggle Relay 2
void handleToggle2() {
  toggleRelay(RELAY2, relay2State);
  handleRoot();
}

void setup() {
  Serial.begin(115200);

  // Connect to WiFi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi");
  Serial.println(WiFi.localIP());

  // Set GPIO modes
  pinMode(RELAY1, OUTPUT);
  pinMode(RELAY2, OUTPUT);
  digitalWrite(RELAY1, relay1State ? HIGH : LOW);
  digitalWrite(RELAY2, relay2State ? HIGH : LOW);

  // Web Server Routes
  server.on("/", handleRoot);
  server.on("/toggle1", handleToggle1);
  server.on("/toggle2", handleToggle2);

  // Start Server
  server.begin();
  Serial.println("Web server started!");
  
}

void loop() {
  server.handleClient();
}
