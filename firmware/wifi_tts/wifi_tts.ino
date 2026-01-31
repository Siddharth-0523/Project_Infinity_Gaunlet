#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "ESP32_TALK";
const char* password = "12345678";

WebServer server(80);

// This will store what you type in Serial Monitor
String message = "Type something in Serial Monitor";

void handleRoot() {
  String page = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <title>ESP32 TTS</title>
</head>
<body style="text-align:center; font-family:Arial;">
  <h2>ESP32 Serial to Speech</h2>
  <p>Message:</p>
  <h3>)rawliteral" + message + R"rawliteral(</h3>

  <button onclick="speak()" style="font-size:24px; padding:15px;">
    🔊 SPEAK
  </button>

  <script>
    function speak() {
      let msg = new SpeechSynthesisUtterance(")rawliteral" + message + R"rawliteral(");
      speechSynthesis.speak(msg);
    }
  </script>

</body>
</html>
)rawliteral";

  server.send(200, "text/html", page);
}

void setup() {
  Serial.begin(115200);
  Serial.println("Type text and press ENTER:");

  WiFi.softAP(ssid, password);
  Serial.print("ESP32 IP: ");
  Serial.println(WiFi.softAPIP());

  server.on("/", handleRoot);
  server.begin();
}

void loop() {
  server.handleClient();

  // Read Serial input
  if (Serial.available()) {
    message = Serial.readStringUntil('\n');
    message.trim();   // remove extra spaces/newlines
    Serial.print("Updated message: ");
    Serial.println(message);
  }
}
