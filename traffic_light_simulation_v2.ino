unsigned long previousMillisTraffic = 0;  // Store last time for traffic lights
unsigned long previousMillisWhite = 0;    // Store last time for white LED

const long intervalWhite = 1000;     // Interval for white LED blink
const long redDuration = 10000;      // Red light duration (10 sec)
const long yellowDuration = 3000;    // Yellow light duration (3 sec)
const long greenDuration = 15000;    // Green light duration (15 sec)
const long yellowAfterGreen = 4000;  // Yellow after green duration (4 sec)

int trafficState = 0;                 // To track the state of the traffic light
unsigned long trafficCycleStart = 0;  // To store the starting time of the traffic light cycle

void setup() {
  Serial.begin(9600);

  pinMode(2, OUTPUT);   // white
  pinMode(9, OUTPUT);   // red
  pinMode(10, OUTPUT);  // yellow
  pinMode(11, OUTPUT);  // green

  // Initialize traffic light to red
  digitalWrite(9, HIGH);         // Red on
  trafficCycleStart = millis();  // Record the start time
}

void loop() {
  trafficLightLoop();
  whiteLedLoop();
}

void whiteLedLoop() {
  int whiteLed = 2;
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillisWhite >= intervalWhite) {
    previousMillisWhite = currentMillis;

    int state = digitalRead(whiteLed);
    digitalWrite(whiteLed, !state);

    Serial.print("Sensor Value: ");
    Serial.print(state);  // Print in seconds
    Serial.println(" seconds");
  }
}

void trafficLightLoop() {
  unsigned long currentMillis = millis();

  switch (trafficState) {
    case 0:  // Red light
      if (currentMillis - trafficCycleStart >= redDuration) {
        digitalWrite(9, LOW);    // Turn off red
        digitalWrite(10, HIGH);  // Turn on yellow
        trafficState = 1;
        trafficCycleStart = currentMillis;  // Reset the timer
      }
      break;
    case 1:  // Yellow light
      if (currentMillis - trafficCycleStart >= yellowDuration) {
        digitalWrite(10, LOW);   // Turn off yellow
        digitalWrite(11, HIGH);  // Turn on green
        trafficState = 2;
        trafficCycleStart = currentMillis;
      }
      break;
    case 2:  // Green light
      if (currentMillis - trafficCycleStart >= greenDuration) {
        digitalWrite(11, LOW);   // Turn off green
        digitalWrite(10, HIGH);  // Turn on yellow
        trafficState = 3;
        trafficCycleStart = currentMillis;
      }
      break;
    case 3:  // Yellow after green
      if (currentMillis - trafficCycleStart >= yellowAfterGreen) {
        digitalWrite(10, LOW);  // Turn off yellow
        digitalWrite(9, HIGH);  // Turn on red
        trafficState = 0;
        trafficCycleStart = currentMillis;
      }
      break;
  }
}