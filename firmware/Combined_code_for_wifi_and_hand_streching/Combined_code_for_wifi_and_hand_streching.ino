#include <WiFi.h>
#include <WebServer.h>

// -------- WIFI SETTINGS --------
const char* ssid = "ESP32_TALK";
const char* password = "12345678";

WebServer server(80);

// -------- FLEX SENSOR SETTINGS --------
const int FLEX_PIN = 34;
const int NORMAL_VALUE = 1264;  // Adjust after calibration
const int TOLERANCE = 30;

String message = "Waiting for gesture...";
unsigned long lastReadTime = 0;

// -------- WEBPAGE HANDLER --------
void handleRoot() {

  String page = "<!DOCTYPE html><html><head><title>Infinity Gauntlet</title></head>";
  page += "<body style='text-align:center;font-family:Arial;'>";
  page += "<h2>Infinity Gauntlet Gesture</h2>";
  page += "<h3>" + message + "</h3>";
  page += "<button onclick='speak()' style='font-size:22px;padding:10px;'>SPEAK</button>";
  page += "<script>";
  page += "function speak(){";
  page += "let msg = new SpeechSynthesisUtterance('" + message + "');";
  page += "speechSynthesis.speak(msg);}";
  page += "</script></body></html>";

  server.send(200, "text/html", page);
}

void setup() {
  Serial.begin(115200);
  pinMode(FLEX_PIN, INPUT);

  // Start WiFi Access Point
  WiFi.softAP(ssid, password);

  Serial.println("WiFi Started");
  Serial.print("IP Address: ");
  Serial.println(WiFi.softAPIP());

  server.on("/", handleRoot);
  server.begin();
}

void loop() {

  server.handleClient();

  // Read flex sensor every 200 ms (non-blocking)
  if (millis() - lastReadTime > 200) {
    lastReadTime = millis();

    long sum = 0;
    for (int i = 0; i < 20; i++) {
      sum += analogRead(FLEX_PIN);
    }

    int currentVal = sum / 20;

    if (currentVal > (NORMAL_VALUE + TOLERANCE)) {
      message = "Closing Hand";
    }
    else if (currentVal < (NORMAL_VALUE - TOLERANCE)) {
      message = "Stretching Hand";
    }
    else {
      message = "Hand Straight";
    }

    Serial.print("Flex Value: ");
    Serial.print(currentVal);
    Serial.print(" | ");
    Serial.println(message);
  }
}