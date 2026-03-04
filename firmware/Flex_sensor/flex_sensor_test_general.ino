/* * Flex Sensor Smoothed Test
 * Uses Averaging to remove ADC jitter
 */

const int FLEX_PIN = 34; 
const int SAMPLES = 20; // Number of samples to average

void setup() {
  Serial.begin(115200);
  pinMode(FLEX_PIN, INPUT);
  Serial.println("--- Stabilized Flex Sensor Test ---");
}

void loop() {
  long sum = 0;

  // Take multiple samples
  for (int i = 0; i < SAMPLES; i++) {
    sum += analogRead(FLEX_PIN);
    delay(2); // Tiny delay between samples
  }

  // Calculate the average
  int averageValue = sum / SAMPLES;
  
  // Convert to Voltage for display
  float voltage = averageValue * (3.3 / 4095.0);

  Serial.print("Stabilized Value: ");
  Serial.print(averageValue);
  Serial.print("\t Voltage: ");
  Serial.println(voltage);

  delay(1000); 
}