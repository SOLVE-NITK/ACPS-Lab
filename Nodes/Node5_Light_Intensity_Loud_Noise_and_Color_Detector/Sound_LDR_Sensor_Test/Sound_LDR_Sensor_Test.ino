#define LM393_PIN A2
#define LDR_PIN A1
#define LED1 0
#define LED2 1

int soundDetected, lightDetected;
int soundStatus, lightStatus;

void setup() {
  Serial.begin(115200);

  pinMode(LM393_PIN, INPUT);
  pinMode(LDR_PIN, INPUT);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
}

void loop() {
  soundDetected = digitalRead(LM393_PIN);
  lightDetected = !digitalRead(LDR_PIN);

  if (!soundDetected) {
    Serial.println("Loud Sound Detected");
    digitalWrite(LED1, HIGH);
    soundStatus = 1;
  } else {
    digitalWrite(LED1, LOW);
    soundStatus = 0;
  }

  if (!lightDetected) {
    Serial.println("Light not Detected");
    digitalWrite(LED2, HIGH);
    soundStatus = 1;
  } else {
    digitalWrite(LED2, LOW);
    soundStatus = 0;
  }
}
