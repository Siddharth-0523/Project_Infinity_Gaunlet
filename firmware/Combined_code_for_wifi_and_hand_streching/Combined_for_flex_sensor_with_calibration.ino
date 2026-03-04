/* 
 * Infinity Gauntlet - Flex Gesture Detection
 * 4 Flex Sensors with Auto Calibration
 * First 5 seconds → detect normal values
 * Then print 1 if bent, 0 if straight
 */

const int INDEX_PIN  = 34;
const int MIDDLE_PIN = 35;
const int RING_PIN   = 32;
const int LITTLE_PIN = 33;

const int SAMPLES = 20;
const int TOLERANCE = 30;

int normalIndex = 0;
int normalMiddle = 0;
int normalRing = 0;
int normalLittle = 0;

// Function to get smoothed reading
int readFlex(int pin) {
  long sum = 0;

  for (int i = 0; i < SAMPLES; i++) {
    sum += analogRead(pin);
    delay(2);
  }

  return sum / SAMPLES;
}

void setup() {

  Serial.begin(115200);

  pinMode(INDEX_PIN, INPUT);
  pinMode(MIDDLE_PIN, INPUT);
  pinMode(RING_PIN, INPUT);
  pinMode(LITTLE_PIN, INPUT);

  Serial.println("Keep fingers straight...");
  Serial.println("Calibrating for 5 seconds");

  long sumIndex = 0;
  long sumMiddle = 0;
  long sumRing = 0;
  long sumLittle = 0;

  int count = 0;

  unsigned long startTime = millis();

  while (millis() - startTime < 5000) {

    sumIndex  += readFlex(INDEX_PIN);
    sumMiddle += readFlex(MIDDLE_PIN);
    sumRing   += readFlex(RING_PIN);
    sumLittle += readFlex(LITTLE_PIN);

    count++;
  }

  normalIndex  = sumIndex / count;
  normalMiddle = sumMiddle / count;
  normalRing   = sumRing / count;
  normalLittle = sumLittle / count;

  Serial.println("Calibration Complete");
  Serial.println("Normal Values:");

  Serial.print("Index: "); Serial.println(normalIndex);
  Serial.print("Middle: "); Serial.println(normalMiddle);
  Serial.print("Ring: "); Serial.println(normalRing);
  Serial.print("Little: "); Serial.println(normalLittle);

  Serial.println("----------------------");
}

void loop() {

  int indexVal  = readFlex(INDEX_PIN);
  int middleVal = readFlex(MIDDLE_PIN);
  int ringVal   = readFlex(RING_PIN);
  int littleVal = readFlex(LITTLE_PIN);

  int indexBent  = abs(indexVal  - normalIndex)  > TOLERANCE ? 1 : 0;
  int middleBent = abs(middleVal - normalMiddle) > TOLERANCE ? 1 : 0;
  int ringBent   = abs(ringVal   - normalRing)   > TOLERANCE ? 1 : 0;
  int littleBent = abs(littleVal - normalLittle) > TOLERANCE ? 1 : 0;

  Serial.print("Index: ");
  Serial.print(indexBent);

  Serial.print("  Middle: ");
  Serial.print(middleBent);

  Serial.print("  Ring: ");
  Serial.print(ringBent);

  Serial.print("  Little: ");
  Serial.println(littleBent);

  delay(200);
}