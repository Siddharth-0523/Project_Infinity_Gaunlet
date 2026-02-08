const int FLEX_PIN = 34;

// --- CALIBRATION LIMITS (Change these based on your serial monitor tests) ---
const int NORMAL_VALUE = 1560; // Your value when the finger is straight
const int TOLERANCE = 30;     // The "Deadzone" to ignore small jitters

void setup() {
  Serial.begin(115200);
  pinMode(FLEX_PIN, INPUT);
  Serial.println("--- Directional Flex Test ---");
}

void loop() {
  // 1. Get a smoothed average reading
  long sum = 0;
  for (int i = 0; i < 20; i++) {
    sum += analogRead(FLEX_PIN);
    delay(2);
  }
  int currentVal = sum / 20;

  // 2. Logic to determine direction
  Serial.print("Value: ");
  Serial.print(currentVal);
  Serial.print(" | Status: ");

  if (currentVal > (NORMAL_VALUE + TOLERANCE)) {
    Serial.println("BENT INWARDS (Closing Hand)");
  } 
  else if (currentVal < (NORMAL_VALUE - TOLERANCE)) {
    Serial.println("BENT OUTWARDS (Stretching)");
  } 
  else {
    Serial.println("NORMAL (Straight)");
  }

  delay(200);
}