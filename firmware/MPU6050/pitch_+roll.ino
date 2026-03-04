#include <Wire.h>
#include <math.h>

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

  roll  = atan2(-axg, azg) * 180.0 / PI;
  pitch = atan2(ayg, azg) * 180.0 / PI;
}

void setup() {
  Serial.begin(115200);
  Wire.begin(21, 22);

  // Wake MPU6050
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x6B);
  Wire.write(0x00);
  Wire.endTransmission(true);

  Serial.println("MPU6050 Ready");
}

void loop() {

  readAngles();

  Serial.print("Roll: ");
  Serial.print(roll);

  Serial.print("   Pitch: ");
  Serial.println(pitch);

  // Gesture detection
  if (pitch > 50) {
    Serial.println("⬆️ UP");
  }
  else if (pitch < -50) {
    Serial.println("⬇️ DOWN");
  }
  else if (roll > 50) {
    Serial.println("➡️ RIGHT");
  }
  else if (roll < -50) {
    Serial.println("⬅️ LEFT");
  }
  else {
    Serial.println("— NEUTRAL");
  }

  delay(200);
}