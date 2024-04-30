#include <MPU9250.h>

MPU9250 mpu;

#define LED_PIN 18        // LED connected to digital pin D18
#define BUZZER_PIN 5      // Buzzer connected to digital pin D5
#define BUZZER_VOLUME 10  // Adjust this value to control the buzzer BUZZER_VOLUME

#define DEBOUNCE_TIME 3000
#define BLINK_INTERVAL 250

float accelX_g, accelY_g, accelZ_g, accelMagnitude;
float roll_deg, pitch_deg, yaw_deg;

bool buzzerState = false;

void actuate() {
  Serial.println("Actuated");

  unsigned long startMillis = millis();
  unsigned long previousMillis = millis();

  unsigned long interval = BLINK_INTERVAL;
  unsigned long duration = DEBOUNCE_TIME;

  while (previousMillis - startMillis < duration) {
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
  Wire.begin();
  Serial.begin(115200);
  while (!Serial)
    ;

  if (!mpu.setup(0x68)) {  // change to your own address
    while (1) {
      Serial.println("MPU connection failed. Please check your connection with `connection_check` example.");
      delay(5000);
    }
  }

  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  digitalWrite(LED_PIN, LOW);
  analogWrite(BUZZER_PIN, 0);
}

void loop() {

  if (mpu.update()) {
    // Read accelerometer data
    accelX_g = mpu.getAccX();
    accelY_g = mpu.getAccY();
    accelZ_g = mpu.getAccZ();
    accelMagnitude = sqrt(accelX_g * accelX_g + accelY_g * accelY_g + accelZ_g * accelZ_g);
  }


  // Print accelerometer data and orientation values
  // Serial.print("Acceleration (g): X=");
  // Serial.print(accelX_g);
  // Serial.print(", Y=");
  // Serial.print(accelY_g);
  // Serial.print(", Z=");
  // Serial.println(accelZ_g);

  // Adjust the free fall and crash threshold values based on your requirements
  float freeFallThreshold = 0.2;  // You may need to fine-tune this value
  float crashThreshold = 2.0;     // You may need to fine-tune this value

  if (accelMagnitude < freeFallThreshold) {
    Serial.println("Free fall detected!");
    actuate();
  }

  if (accelMagnitude > crashThreshold) {
    Serial.println("Crash detected!");
    actuate();
  }

  if (mpu.update()) {
    // Read orientation values
    roll_deg = mpu.getRoll();
    pitch_deg = mpu.getPitch();
    yaw_deg = mpu.getYaw();

    Serial.print("Orientation (degrees): Roll=");
    Serial.print(roll_deg);
    Serial.print(", Pitch=");
    Serial.print(pitch_deg);
    Serial.print(", Yaw=");
    Serial.println(yaw_deg);
  }

  delay(100);  // Adjust the delay based on your required sampling rate
}
