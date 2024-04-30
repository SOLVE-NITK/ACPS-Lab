#include <Wire.h>
#include <ACS712.h>
#include <CytronMotorDriver.h>
#include <LiquidCrystal_I2C.h>

#define VOLTAGE_SENSOR 32
#define CURRENT_SENSOR 35

#define FAN_SWITCH 12
#define LIGHT_SWITCH 13
#define HEATER_SWITCH 19
#define FAN_SPEED_KNOB 33

#define MOTOR_DIR 5
#define MOTOR_PWM 18

#define LIGHT_RELAY 2
#define HEATER_RELAY 4

ACS712 ACS(CURRENT_SENSOR, 3.3, 4095, 100);

LiquidCrystal_I2C lcd(0x3F, 16, 2);

CytronMD motor(PWM_DIR, MOTOR_PWM, MOTOR_DIR);

#define VOLTAGE_ERROR_FACTOR 0.59;

// Floats for ADC voltage & Input voltage
float adc_voltage = 0.0;
float in_voltage = 0.0;

// Floats for resistor values in divider (in ohms)
float R1 = 30000.0;
float R2 = 7500.0;

// Float for Reference Voltage
float ref_voltage = 3.3;

// Integer for ADC value
int adc_value = 0;

float voltage = 0.0, current = 0.0, power = 0.0, energy = 0.0;

bool fanSwitchPressed = false, lightSwitchPressed = false, heaterSwitchPressed = false;

//variables to keep track of the timing of recent interrupts
unsigned long switch_time = 0;
unsigned long last_fan_switch_time = 0;
unsigned long last_light_switch_time = 0;
unsigned long last_heater_switch_time = 0;

unsigned long lcdUpdateTimer = 0;
const unsigned long lcdUpdateInterval = 2000;  // Update every 2 seconds
bool displayingVoltageCurrent = true;          // Display voltage and current initially

void IRAM_ATTR fanSwitchCheck() {
  switch_time = millis();
  if (switch_time - last_fan_switch_time > 250) {
    fanSwitchPressed = !fanSwitchPressed;
    last_fan_switch_time = switch_time;
  }
}

void IRAM_ATTR lightSwitchCheck() {
  switch_time = millis();
  if (switch_time - last_light_switch_time > 250) {
    lightSwitchPressed = !lightSwitchPressed;
    last_light_switch_time = switch_time;
  }
}

void IRAM_ATTR heaterSwitchCheck() {
  switch_time = millis();
  if (switch_time - last_heater_switch_time > 250) {
    heaterSwitchPressed = !heaterSwitchPressed;
    last_heater_switch_time = switch_time;
  }
}

void turnOnLight() {
  digitalWrite(LIGHT_RELAY, HIGH);
}

void turnOffLight() {
  digitalWrite(LIGHT_RELAY, LOW);
}

void turnOnHeater() {
  digitalWrite(HEATER_RELAY, HIGH);
}

void turnOffHeater() {
  digitalWrite(HEATER_RELAY, LOW);
}

void turnOnFan() {
  motor.setSpeed(255);
}

void turnOffFan() {
  motor.setSpeed(0);
}

float getVoltage() {
  // Read the Analog Input
  adc_value = analogRead(VOLTAGE_SENSOR);

  // Determine voltage at ADC input
  adc_voltage = (adc_value * ref_voltage) / 4095.0;

  // Calculate voltage at divider input
  in_voltage = adc_voltage * (R1 + R2) / R2;

  in_voltage = in_voltage + VOLTAGE_ERROR_FACTOR;

  return in_voltage;
}

int getCurrent() {
  int mA = ACS.mA_DC();
  return mA;
}

float getPower() {
  float tempPower = getVoltage() * getCurrent();
  return tempPower;
}

float getEnergy() {
  float tempEnergy = getPower();
  return tempEnergy;
}

void updateLCD(float voltage, int current, float power, float energy) {
  unsigned long currentMillis = millis();

  if (currentMillis - lcdUpdateTimer >= lcdUpdateInterval) {
    lcdUpdateTimer = currentMillis;  // Reset the timer

    lcd.clear();  // Clear the display

    if (displayingVoltageCurrent) {
      lcd.setCursor(0, 0);
      lcd.print("V: ");
      lcd.print(voltage, 2);
      lcd.print(" V   ");

      lcd.setCursor(0, 1);
      lcd.print("I: ");
      lcd.print(current);
      lcd.print(" mA");
    } else {
      lcd.setCursor(0, 0);
      lcd.print("P: ");
      lcd.print(power);
      lcd.print(" mW");

      lcd.setCursor(0, 1);
      lcd.print("E: ");
      lcd.print(energy);
      lcd.print(" mWH");
    }

    displayingVoltageCurrent = !displayingVoltageCurrent;  // Toggle between voltage/current and power/energy
  }
}


void setup() {

  Serial.begin(115200);

  pinMode(FAN_SWITCH, INPUT_PULLUP);
  pinMode(LIGHT_SWITCH, INPUT_PULLUP);
  pinMode(HEATER_SWITCH, INPUT_PULLUP);

  attachInterrupt(FAN_SWITCH, fanSwitchCheck, FALLING);
  attachInterrupt(LIGHT_SWITCH, lightSwitchCheck, FALLING);
  attachInterrupt(HEATER_SWITCH, heaterSwitchCheck, FALLING);

  pinMode(LIGHT_RELAY, OUTPUT);
  pinMode(HEATER_RELAY, OUTPUT);

  motor.setSpeed(0);

  digitalWrite(LIGHT_RELAY, LOW);
  digitalWrite(HEATER_RELAY, LOW);

  ACS.autoMidPoint();

  lcd.init();
  lcd.backlight();
}

void loop() {

  voltage = getVoltage();
  current = getCurrent();
  power = getPower();
  energy = getEnergy();

  updateLCD(voltage, current, power, energy);

  Serial.print("Voltage = ");
  Serial.print(voltage, 2);
  Serial.println(" V");

  Serial.print("Current = ");
  Serial.print(current);
  Serial.println(" mA");

  Serial.print("Power = ");
  Serial.print(power);
  Serial.println(" mW");

  Serial.print("Energy = ");
  Serial.print(energy);
  Serial.println(" mWH");

  Serial.println();

  if (fanSwitchPressed) {
    Serial.println("Fan is Turned On");
    turnOnFan();
  } else {
    Serial.println("Fan is Turned Off");
    turnOffFan();
  }

  if (lightSwitchPressed) {
    Serial.println("Light is Turned On");
    turnOnLight();
  } else {
    Serial.println("Light is Turned Off");
    turnOffLight();
  }

  if (heaterSwitchPressed) {
    Serial.println("Heater is Turned On");
    turnOnHeater();
  } else {
    Serial.println("Heater is Turned Off");
    turnOffHeater();
  }

  Serial.println();

  delay(100);
}