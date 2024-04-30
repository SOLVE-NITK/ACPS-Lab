#include <Servo.h>

// Set GPIOs for LED_PIN and PIR Motion Sensor
#define LED_PIN 12
#define PIR_PIN 14
#define SERVO_PIN 13

Servo myservo;

//variables to keep track of the timing of recent interrupts
unsigned long button_time = 0;
unsigned long last_button_time = 0;

#define CLOSE_POS 180
#define OPEN_POS 40

#define DEBOUNCE_TIME 6000

bool motionDetected = false;

int pos = 0;

// Checks if motion was detected
ICACHE_RAM_ATTR void detectMotion() {
  button_time = millis();
  if (button_time - last_button_time > DEBOUNCE_TIME) {
    motionDetected = true;
    last_button_time = button_time;
  }
}

void openDoor() {
  for (pos = CLOSE_POS; pos >= OPEN_POS; pos -= 10) { 
    myservo.write(pos);                                
    delay(1);                                          
  }
}

void closeDoor() {
  for (pos = OPEN_POS; pos <= CLOSE_POS; pos += 10) {  
    myservo.write(pos);                                
    delay(1);                                          
  }
}

void setup() {
  // Serial port for debugging purposes
  Serial.begin(115200);

  // PIR Motion Sensor mode INPUT_PULLUP
  pinMode(PIR_PIN, INPUT_PULLUP);
  // Set PIR_PIN pin as interrupt, assign interrupt function and set RISING mode
  attachInterrupt(digitalPinToInterrupt(PIR_PIN), detectMotion, RISING);

  myservo.attach(SERVO_PIN);
  pinMode(LED_PIN, OUTPUT);

  myservo.write(CLOSE_POS);
  digitalWrite(LED_PIN, LOW);
}

void loop() {

  if (motionDetected) {
    Serial.println("Motion Detected!");
    digitalWrite(LED_PIN, HIGH);
    openDoor();
    delay(3000);
    closeDoor();
    motionDetected = false;
  } else {
    digitalWrite(LED_PIN, LOW);
    myservo.write(CLOSE_POS);
  }
}