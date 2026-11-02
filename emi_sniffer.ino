// DIY EMI Sniffer - LED Bar Graph Display
const int antennaPin = A0;             // EMI Probe Pin
const int ledPins[] = {2, 3, 4, 5};   // Digital pins connected to LEDs
const int numLeds = 4;

// Threshold values for lighting up each LED level (0 to 1023 scale)
// Adjust these numbers if your LEDs turn on too easily or not enough
int thresholds[] = {2, 5, 10, 20}; 

void setup() {
  Serial.begin(9600);
  
  // Set all LED pins as OUTPUT
  for (int i = 0; i < numLeds; i++) {
    pinMode(ledPins[i], OUTPUT);
  }
}

void loop() {
  int sensorValue = analogRead(antennaPin); // Read raw EMI noise level
  
  // Print value to Serial Monitor/Plotter for real-time debugging
  Serial.println(sensorValue);

  // Check noise level against thresholds and update LEDs
  for (int i = 0; i < numLeds; i++) {
    if (sensorValue >= thresholds[i]) {
      digitalWrite(ledPins[i], HIGH); // Turn LED ON
    } else {
      digitalWrite(ledPins[i], LOW);  // Turn LED OFF
    }
  }
  
  delay(20); // Smooth update rate
}
