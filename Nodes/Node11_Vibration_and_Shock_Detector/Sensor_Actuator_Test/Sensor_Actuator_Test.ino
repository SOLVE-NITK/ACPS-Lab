#define VIBRATION_SENSOR 2
#define LED_PIN 4
#define BUZZER_PIN 5

#define BUZZER_VOLUME 10

#define DEBOUNCE_TIME 3000
#define BLINK_INTERVAL 250

//variables to keep track of the timing of recent interrupts
unsigned long button_time = 0;
unsigned long last_button_time = 0;

bool vibrationDetected = false, buzzerState = false;

void detectVibration() {
  button_time = millis();
  if (button_time - last_button_time > DEBOUNCE_TIME) {
    vibrationDetected = true;
    last_button_time = button_time;
  }
}

void actuate(int milliSeconds) {
  unsigned long previousMillis = 0;
  unsigned long interval = BLINK_INTERVAL;

  unsigned long duration = milliSeconds;

  while (millis() - previousMillis < duration) {
    if (millis() - previousMillis >= interval) {
      previousMillis = millis();
      digitalWrite(LED_PIN, !digitalRead(LED_PIN));
      if (buzzerState == false) {
        analogWrite(BUZZER_PIN, BUZZER_VOLUME);
        buzzerState = true;
      } else {
        analogWrite(BUZZER_PIN, 0);
        buzzerState = false;
      }
    }
  }
  digitalWrite(LED_PIN, LOW);
  analogWrite(BUZZER_PIN, 0);
}

void setup() {
  Serial.begin(115200);

  pinMode(VIBRATION_SENSOR, INPUT_PULLUP);
  attachInterrupt(VIBRATION_SENSOR, detectVibration, FALLING);

  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
}

void loop() {

  if (vibrationDetected) {
    actuate(DEBOUNCE_TIME);
    vibrationDetected = false;
  } else {
    digitalWrite(LED_PIN, LOW);
    analogWrite(BUZZER_PIN, 0);
  }
}