#include <ThingsBoard.h>
#include <WiFi.h>
#include <Wire.h>
#include <Adafruit_INA219.h>
#include <CytronMotorDriver.h>
#include <LiquidCrystal_I2C.h>

#define HEATER_SWITCH 12
#define LIGHT_SWITCH 13
#define FAN_SWITCH 19
#define FAN_SPEED_KNOB 33

#define MOTOR_DIR 5
#define MOTOR_PWM 18

#define LIGHT_RELAY 2
#define HEATER_RELAY 4

#define WIFI_STATUS_LED 2

Adafruit_INA219 ina219;

LiquidCrystal_I2C lcd(0x3F, 16, 2);

CytronMD motor(PWM_DIR, MOTOR_PWM, MOTOR_DIR);

int critical_state = 0;

constexpr char WIFI_SSID[] PROGMEM = "CSD";
constexpr char WIFI_PASSWORD[] PROGMEM = "csd@NITK2014";

constexpr char TOKEN[] PROGMEM = "x3xPlF8GT9v1sckO1r1Q";

constexpr char THINGSBOARD_SERVER[] PROGMEM = "10.14.0.205";
constexpr uint16_t THINGSBOARD_PORT PROGMEM = 1883U;
constexpr uint32_t MAX_MESSAGE_SIZE PROGMEM = 128U;
constexpr uint32_t SERIAL_DEBUG_BAUD PROGMEM = 115200U;

constexpr const char RPC_POWER_DATA[] PROGMEM = "power";
constexpr const char RPC_CURRENT_DATA[] PROGMEM = "current";
constexpr const char RPC_VOLTAGE_DATA[] PROGMEM = "voltage";
constexpr const char RPC_LOAD_VOLTAGE_DATA[] PROGMEM = "load voltage";

constexpr const char RPC_CRITICAL_METHOD[] PROGMEM = "critical";

constexpr char LIGHT_KEY[] PROGMEM = "light";
constexpr char FAN_KEY[] PROGMEM = "fan";
constexpr char HEATER_KEY[] PROGMEM = "heater";

constexpr const char FAN_TELEMETRY[] PROGMEM = "Fan_ON";
constexpr const char LIGHT_TELEMETRY[] PROGMEM = "Light_ON";
constexpr const char HEATER_TELEMETRY[] PROGMEM = "Heater_ON";

float shuntvoltage = 0;
float busvoltage = 0;
float current_mA = 0;
float loadvoltage = 0;
float power_mW = 0;

bool fanSwitchPressed = false, lightSwitchPressed = false, heaterSwitchPressed = false;

int fanSpeed = 0;

//variables to keep track of the timing of recent interrupts
unsigned long switch_time = 0;
unsigned long last_fan_switch_time = 0;
unsigned long last_light_switch_time = 0;
unsigned long last_heater_switch_time = 0;

unsigned long lcdUpdateTimer1 = 0;
const unsigned long lcdUpdateInterval1 = 3000;

unsigned long lcdUpdateTimer = 0;
const unsigned long lcdUpdateInterval = 2000;  // Update every 2 seconds
bool displayingVoltageCurrent = true;          // Display voltage and current initially

WiFiClient espClient;
ThingsBoard tb(espClient, MAX_MESSAGE_SIZE);

int status = WL_IDLE_STATUS;  // the Wifi radio's status
bool subscribed = false;

/// @brief Initalizes WiFi connection,
// will endlessly delay until a connection has been successfully established
void InitWiFi() {

  Serial.print("Attempting to connect to network: ");
  Serial.println(WIFI_SSID);

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("Connected to AP");
}

/// Reconnects the WiFi uses InitWiFi if the connection has been removed
/// Returns true as soon as a connection has been established again
bool reconnect() {
  // Check to ensure we aren't connected yet
  const wl_status_t status = WiFi.status();
  if (status == WL_CONNECTED) {
    return true;
  }

  // If we aren't establish a new connection to the given WiFi network
  InitWiFi();
  return true;
}

RPC_Response processcritical_state(RPC_Data &data) {
  Serial.println("RECIEVED FREEFALL STATE");
  critical_state = data;
  Serial.println("FREEFALL STATE CHANGE:");
  Serial.print(critical_state);
  return RPC_Response("actuatorState", critical_state);
}

const std::array<RPC_Callback, 2U> callbacks = {
  RPC_Callback{ RPC_CRITICAL_METHOD, processcritical_state }
};


void IRAM_ATTR fanSwitchCheck() {
  switch_time = millis();
  if (switch_time - last_fan_switch_time > 1000) {
    fanSwitchPressed = !fanSwitchPressed;
    last_fan_switch_time = switch_time;
  }
}

void IRAM_ATTR lightSwitchCheck() {
  switch_time = millis();
  if (switch_time - last_light_switch_time > 1000) {
    lightSwitchPressed = !lightSwitchPressed;
    last_light_switch_time = switch_time;
  }
}

void IRAM_ATTR heaterSwitchCheck() {
  switch_time = millis();
  if (switch_time - last_heater_switch_time > 1000) {
    heaterSwitchPressed = !heaterSwitchPressed;
    last_heater_switch_time = switch_time;
  }
}

void turnOnLight() {
  digitalWrite(LIGHT_RELAY, HIGH);
  tb.sendTelemetryString(LIGHT_TELEMETRY, "Light is On");
  tb.sendAttributeBool(LIGHT_KEY, true);
}

void turnOffLight() {
  digitalWrite(LIGHT_RELAY, LOW);
  tb.sendTelemetryString(LIGHT_TELEMETRY, "Light is Off");
  tb.sendAttributeBool(LIGHT_KEY, false);
}

void turnOnHeater() {
  digitalWrite(HEATER_RELAY, HIGH);
  tb.sendTelemetryString(HEATER_TELEMETRY, "Heater is On");
  tb.sendAttributeBool(HEATER_KEY, true);
}

void turnOffHeater() {
  digitalWrite(HEATER_RELAY, LOW);
  tb.sendTelemetryString(HEATER_TELEMETRY, "Heater is Off");
  tb.sendAttributeBool(HEATER_KEY, false);
}

void turnOnFan() {
  fanSpeed = analogRead(FAN_SPEED_KNOB);
  tb.sendTelemetryString(FAN_TELEMETRY, "Fan is On");
  tb.sendAttributeBool(FAN_KEY, true);
  motor.setSpeed(map(fanSpeed, 0, 4095, 30, 70));
}

void turnOffFan() {
  tb.sendTelemetryString(FAN_TELEMETRY, "Fan is Off");
  tb.sendAttributeBool(FAN_KEY, false);

  motor.setSpeed(0);
}

void updateLCD_display() {
  unsigned long currentMillis = millis();

  if (currentMillis - lcdUpdateTimer1 >= lcdUpdateInterval1) {
    lcdUpdateTimer1 = currentMillis;  // Reset the timer

    lcd.clear();  // Clear the display

    if (displayingVoltageCurrent) {
      lcd.setCursor(0, 0);
      lcd.print("EMERGENCY ALERT");
    }
  }
}

void updateLCD(float voltage, int current, float power) {
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
    }

    displayingVoltageCurrent = !displayingVoltageCurrent;  // Toggle between voltage/current and power/energy
  }
}

void setup() {
  // Initalize serial connection for debugging
  Serial.begin(SERIAL_DEBUG_BAUD);
  Wire.begin();
  delay(1000);
  InitWiFi();

  pinMode(WIFI_STATUS_LED, OUTPUT);
  digitalWrite(WIFI_STATUS_LED, LOW);

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

  if (!ina219.begin()) {
    Serial.println("Failed to find INA219 chip");
    while (1) { delay(10); }
  }

  lcd.init();
  lcd.backlight();
}

void loop() {
  delay(100);

  if (!reconnect()) {
    return;
  }

  if (!tb.connected()) {
    // Reconnect to the ThingsBoard server,
    // if a connection was disrupted or has not yet been established
    Serial.printf("Connecting to: (%s) with token (%s)\n", THINGSBOARD_SERVER, TOKEN);
    digitalWrite(WIFI_STATUS_LED, LOW);
    if (!tb.connect(THINGSBOARD_SERVER, TOKEN, THINGSBOARD_PORT)) {
      Serial.println(F("Failed to connect"));
      return;
    }
  }

  if (!subscribed) {
    Serial.println(F("Subscribing for RPC..."));

    // Perform a subscription. All consequent data processing will happen in
    // processTemperatureChange() and processSwitchChange() functions,
    // as denoted by callbacks array.
    if (!tb.RPC_Subscribe(callbacks.cbegin(), callbacks.cend())) {
      Serial.println(F("Failed to subscribe for RPC"));
      return;
    }
    Serial.println(F("Subscribe done"));
    digitalWrite(WIFI_STATUS_LED, HIGH);

    subscribed = true;
  }

  shuntvoltage = ina219.getShuntVoltage_mV();
  busvoltage = ina219.getBusVoltage_V();
  current_mA = ina219.getCurrent_mA();
  power_mW = ina219.getPower_mW();
  loadvoltage = busvoltage + (shuntvoltage / 1000);

  if (critical_state == 40) {
    critical_state = 0;
    updateLCD_display();
    turnOffFan();
    turnOffHeater();
    turnOffLight();
    delay(1000);
  } else if (critical_state == 30) {
    critical_state = 0;
    turnOnFan();
    turnOffHeater();
    turnOffLight();
    delay(1000);
  } else if (critical_state == 20) {
    critical_state = 0;
    turnOnHeater();
    turnOffFan();
    turnOffLight();
    delay(1000);
  } else if (critical_state == 10) {
    critical_state = 0;
    turnOnLight();
    turnOffFan();
    turnOffHeater();
    delay(1000);
  }

  else {
    updateLCD(busvoltage, current_mA, power_mW);

    Serial.print(analogRead(FAN_SPEED_KNOB));
    Serial.print("\t");
    Serial.print(map(analogRead(FAN_SPEED_KNOB), 0, 4095, 0, 255));
    Serial.println();

    if (fanSwitchPressed) {
      // Serial.println("Fan is Turned On");
      turnOnFan();
    } else {
      // Serial.println("Fan is Turned Off");
      turnOffFan();
    }

    if (lightSwitchPressed) {
      // Serial.println("Light is Turned On");
      turnOnLight();
    } else {
      // Serial.println("Light is Turned Off");
      turnOffLight();
    }

    if (heaterSwitchPressed) {
      // Serial.println("Heater is Turned On");
      turnOnHeater();
    } else {
      // Serial.println("Heater is Turned Off");
      turnOffHeater();
    }
  }

  // tb.sendTelemetryData(RPC_IMPACT_METHOD, power_mW);
  Serial.println("\n");
  Serial.println(power_mW);
  Serial.println("\n");


  tb.sendTelemetryData(RPC_POWER_DATA, power_mW);
  tb.sendTelemetryData(RPC_CURRENT_DATA, current_mA);
  tb.sendTelemetryData(RPC_VOLTAGE_DATA, busvoltage);
  tb.sendTelemetryData(RPC_LOAD_VOLTAGE_DATA, loadvoltage);
  // Uploads new telemetry to ThingsBoard
  // Serial.println(F("Sending data..."));

  // delay(1000);

  tb.loop();
}