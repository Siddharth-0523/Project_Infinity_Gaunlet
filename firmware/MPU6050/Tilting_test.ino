#include <Wire.h>
#include <math.h>

const int MPU_ADDR = 0x68;

float readRoll() {
  int16_t ax, ay, az;

  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x3B);   // Accel start register
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_ADDR, 6, true);

  ax = Wire.read() << 8 | Wire.read();
  ay = Wire.read() << 8 | Wire.read();
  az = Wire.read() << 8 | Wire.read();

  float axg = ax / 16384.0;
  float azg = az / 16384.0;

  // Roll angle in degrees
  return atan2(-axg, azg) * 180.0 / PI;
}

void setup() {
  Serial.begin(115200);
  Wire.begin(21, 22);

  // Wake up MPU6050
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x6B);
  Wire.write(0x00);
  Wire.endTransmission(true);

  Serial.println("MPU6050 ready");
}

void loop() {
  float roll = readRoll();

  if (roll > 50) {
    Serial.println("⬆️ UP");
  }
  else if (roll < -50) {
    Serial.println("⬇️ DOWN");
  }
  else {
    Serial.println("— NEUTRAL");
  }

  Serial.print("Roll = ");
  Serial.println(roll);

  delay(200);
}
