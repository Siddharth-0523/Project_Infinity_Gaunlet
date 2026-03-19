#include <Wire.h>
#include <WiFi.h>
#include <WebServer.h>
#include <math.h>

//////////////////// WIFI ////////////////////

const char* ssid = "ESP32_TALK";
const char* password = "12345678";

WebServer server(80);
String message = "Infinity Gauntlet Ready";

void handleRoot() {
  String page = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <title>Infinity Gauntlet</title>
</head>
<body style="text-align:center; font-family:Arial;">
  <h2>🧤 Infinity Gauntlet Voice Output</h2>
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

//////////////////// FLEX SENSORS ////////////////////

const int INDEX_PIN  = 34;
const int MIDDLE_PIN = 35;
const int RING_PIN   = 32;
const int THUMB_PIN  = 33;

const int SAMPLES = 20;
const int TOLERANCE = 20;

int normalIndex = 0;
int normalMiddle = 0;
int normalRing = 0;
int normalThumb = 0;

int readFlex(int pin) {
  long sum = 0;
  for (int i = 0; i < SAMPLES; i++) {
    sum += analogRead(pin);
    delay(2);
  }
  return sum / SAMPLES;
}

//////////////////// MPU6050 ////////////////////

const int MPU_ADDR = 0x68;
float roll, pitch;

void readAngles() {
  int16_t ax, ay, az;

  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_ADDR, 6, true);

  ax = Wire.read() << 8 | Wire.read();
  ay = Wire.read() << 8 | Wire.read();
  az = Wire.read() << 8 | Wire.read();

  float axg = ax / 16384.0;
  float ayg = ay / 16384.0;
  float azg = az / 16384.0;

  roll  = atan2(ayg, azg) * 180.0 / PI;
  pitch = atan2(-axg, sqrt(ayg * ayg + azg * azg)) * 180.0 / PI;
}

//////////////////// SETUP ////////////////////

void setup() {

  Serial.begin(115200);

  WiFi.softAP(ssid, password);
  Serial.print("ESP32 IP: ");
  Serial.println(WiFi.softAPIP());

  server.on("/", handleRoot);
  server.begin();

  Wire.begin(21, 22);

  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x6B);
  Wire.write(0x00);
  Wire.endTransmission(true);

  // FLEX CALIBRATION
  Serial.println("Keep fingers straight...");
  Serial.println("Calibrating for 5 seconds");

  long sI=0,sM=0,sR=0,sT=0;
  int count=0;

  unsigned long start = millis();

  while (millis() - start < 5000) {
    sI += readFlex(INDEX_PIN);
    sM += readFlex(MIDDLE_PIN);
    sR += readFlex(RING_PIN);
    sT += readFlex(THUMB_PIN);
    count++;
  }

  normalIndex  = sI / count;
  normalMiddle = sM / count;
  normalRing   = sR / count;
  normalThumb  = sT / count;

  Serial.println("Calibration Complete");
}

//////////////////// LOOP ////////////////////

void loop() {

  server.handleClient();

  // ===== FLEX READ =====

  int indexVal  = readFlex(INDEX_PIN);
  int middleVal = readFlex(MIDDLE_PIN);
  int ringVal   = readFlex(RING_PIN);
  int thumbVal  = readFlex(THUMB_PIN);

  int indexBent  = abs(indexVal  - normalIndex)  > TOLERANCE ? 1 : 0;
  int middleBent = abs(middleVal - normalMiddle) > TOLERANCE ? 1 : 0;
  int ringBent   = abs(ringVal   - normalRing)   > TOLERANCE ? 1 : 0;
  int thumbBent  = abs(thumbVal  - normalThumb)  > TOLERANCE ? 1 : 0;

  // ===== MPU READ =====

  readAngles();

  int mpuCode = 0;

  if (roll > 40)        mpuCode = 1;
  else if (roll < -40)  mpuCode = 2;
  else if (pitch < -40) mpuCode = 3;
  else if (pitch > 40)  mpuCode = 4;
  else                  mpuCode = 0;

  // ===== DICTIONARY =====

  if      (thumbBent==0 && indexBent==0 && middleBent==0 && ringBent==0 && mpuCode==0)
      message = "Hand Rest";

  else if (thumbBent==0 && indexBent==0 && middleBent==0 && ringBent==0 && mpuCode==1)
      message = "Hi";

  else if (thumbBent==1 && indexBent==1 && middleBent==1 && ringBent==1 && mpuCode==0)
      message = "This";

  else if (thumbBent==0 && indexBent==1 && middleBent==1 && ringBent==1 && mpuCode==2)
      message = "is";

  else if (thumbBent==1 && indexBent==1 && middleBent==1 && ringBent==1 && mpuCode==2)
      message = "Siddharth";

  else if (thumbBent==0 && indexBent==0 && middleBent==0 && ringBent==0 && mpuCode==4)
      message = "nice to meet you";

  else message = "unspecified";

  // ===== ALWAYS PRINT 4 DIGITS =====

  Serial.print("TIMR: ");
  Serial.print(thumbBent);
  Serial.print(indexBent);
  Serial.print(middleBent);
  Serial.print(ringBent);

  Serial.print("  MPU: ");
  Serial.print(mpuCode);

  Serial.print("  -> ");
  Serial.println(message);

  delay(200);
}