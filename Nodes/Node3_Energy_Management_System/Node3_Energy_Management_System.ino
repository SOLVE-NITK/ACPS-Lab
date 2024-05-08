#include <ThingsBoard.h>
#include <WiFi.h>
#include <ArduinoOTA.h>
#include <ArduinoHttpClient.h>

#include <Wire.h>
#include <Adafruit_INA219.h>
#include <CytronMotorDriver.h>
#include <LiquidCrystal_I2C.h>

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

WiFiClient espClient;
ThingsBoard tb(espClient, MAX_MESSAGE_SIZE);

uint8_t status = WL_IDLE_STATUS;  // the Wifi radio's status
bool subscribed = false;
bool requestedShared = false;
int msg = 0;

char *BASE_URL = "/api/v1";   // Define base URL for API requests
char *ENDPOINT = "firmware";  // Define endpoint for firmware updates
char PATH[256];               // Define array to store the path for firmware updates

constexpr const char FW_TITLE_KEY[] PROGMEM = "fw_title";
constexpr const char FW_VER_KEY[] PROGMEM = "fw_version";

char CURRENT_VERSION[] = "1.0.0";
constexpr int FIRMWARE_SIZE = 20;           // Adjust the size according to your requirements
char NEW_VERSION[FIRMWARE_SIZE] = "1.0.0";  // Declare NEW_VERSION array

char FW_TITLE[] = "ESP32";
constexpr int TITLE_SIZE = 20;       // Adjust the size according to your requirements
char FWW_TITLE[TITLE_SIZE] = "ESP32";  // Declare NEW_VERSION array

// Shared attributes we want to request from the server
constexpr std::array<const char *, 2U> REQUESTED_SHARED_ATTRIBUTES = {
  FW_TITLE_KEY,
  FW_VER_KEY
};

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

int critical_state = 0;

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

bool reconnect() {
  // Check to ensure we aren't connected yet
  status = WiFi.status();
  if (status == WL_CONNECTED) {
    return true;
  }

  // If we aren't establish a new connection to the given WiFi network
  InitWiFi();
  return true;
}

void processSharedAttributeRequest(const Shared_Attribute_Data &data) {
  for (auto it = data.begin(); it != data.end(); ++it) {
    Serial.println(it->key().c_str());
    // Shared attributes have to be parsed by their type.
    Serial.println(it->value().as<const char *>());
    if (strcmp_P(it->key().c_str(), FW_VER_KEY) == 0) {
      // If the key is "CURRENT_VERSION", print its value
      Serial.print("NEW_version: ");
      // Copy the value to NEW_version array
      strncpy(NEW_version, it->value().as<const char *>(), FIRMWARE_SIZE - 1);
      // Ensure null termination
      NEW_version[FIRMWARE_SIZE - 1] = '\0';
      // Print the value
      Serial.println(NEW_version);
    }
    if (strcmp_P(it->key().c_str(), FW_TITLE_KEY) == 0) {
      // If the key is "CURRENT_VERSION", print its value
      Serial.print("FWW_title: ");
      // Copy the value to NEW_version array
      strncpy(FWW_title, it->value().as<const char *>(), TITLE_SIZE - 1);
      // Ensure null termination
      FWW_title[TITLE_SIZE - 1] = '\0';
      // Print the value
      Serial.println(FWW_title);
    }
  }

  int jsonSize = JSON_STRING_SIZE(measureJson(data));
  char buffer[jsonSize];
  serializeJson(data, buffer, jsonSize);
  Serial.println(buffer);
}

const Attribute_Request_Callback sharedCallback(REQUESTED_SHARED_ATTRIBUTES.cbegin(), REQUESTED_SHARED_ATTRIBUTES.cend(), &processSharedAttributeRequest);

void handleSketchDownload(const char *token, char *title, char *CURRENT_VERSION) {
  sprintf(PATH, "%s/%s/%s?title=%s&version=%s", BASE_URL, token, ENDPOINT, title, CURRENT_VERSION);
  // const char* THINGSBOARD_SERVER = "10.100.80.25";  // Set your correct hostname
  const unsigned short SERVER_PORT = 8090U;                       // Commonly 80 (HTTP) | 443 (HTTPS)
  HttpClient client(espClient, THINGSBOARD_SERVER, SERVER_PORT);  // HTTP
  // HttpClient client(wifiClientSSL, SERVER, SERVER_PORT);  // HTTPS
  char buff[64];
  snprintf(buff, sizeof(buff), PATH);      // Copy the URL path to the buffer
  Serial.print("Check for update file ");  // Print message to indicate checking for update file with the URL path
  Serial.println(buff);
  // Make the GET request
  client.get(buff);                              // Make a GET request to the server with the URL path
  int statusCode = client.responseStatusCode();  // Retrieve the HTTP status code from the response
  Serial.print("Update status code: ");          // Print the update status code received from the server
  Serial.println(statusCode);

  if (statusCode != 200) {  // Check if the status code indicates a successful response (HTTP 200 OK)
    client.stop();          // If the status code is not 200, stop the client connection and return
    return;
  }

  long length = client.contentLength();  // Retrieve the content length of the response from the server

  if (length == HttpClient::kNoContentLengthHeader) {                                            // Check if the server provided a Content-Length header
    client.stop();                                                                               // Stop the client connection
    Serial.println("Server didn't provide Content-length header. Can't continue with update.");  // Print error message
    return;                                                                                      // Exit the function as the update cannot proceed without content length information
  }
  Serial.print("Server returned update file of size ");  // Print message indicating successful response from server
  Serial.print(length);                                  // Print the size of the update file in bytes
  Serial.println(" bytes");                              // Print bytes

  if (!InternalStorage.open(length)) {                                                             // Attempt to open InternalStorage to store the update
    client.stop();                                                                                 // Stop the client connection
    Serial.println("There is not enough space to store the update. Can't continue with update.");  // Print error message
    return;                                                                                        // Exit the function as there is insufficient space for the update
  }
  byte b;                          // Declare a variable 'b' of type byte to store data read from the client
  while (length > 0) {             // Start a loop to read bytes from the client until 'length' becomes zero
    if (!client.readBytes(&b, 1))  // reading a byte with timeout
      break;                       // If unable to read a byte within the timeout, exit the loop
    InternalStorage.write(b);      // Write the read byte to InternalStorage
    length--;                      // Decrement the length to keep track of bytes read
  }
  InternalStorage.close();  // Close the InternalStorage after writing all bytes

  client.stop();                                            // Stop the client connection after the update process
  if (length > 0) {                                         // Check if there are remaining bytes to be read
    Serial.print("Timeout downloading update file at ");    // Print error message for timeout
    Serial.print(length);                                   // Print the number of remaining bytes
    Serial.println(" bytes. Can't continue with update.");  // Print error message
    return;                                                 // Exit the function if unable to download the complete update
  }

  Serial.println("Sketch update apply and reset.");  // Print message indicating successful update
  Serial.flush();                                    // Flush the serial buffer to ensure all data is sent
  InternalStorage.apply();                           // this doesn't return
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

  if (!requestedShared) {
    Serial.println("Requesting shared attributes...");
    requestedShared = tb.Shared_Attributes_Request(sharedCallback);
    if (!requestedShared) {
      Serial.println("Failed to request shared attributes");
    }
  }

  if (strcmp(FWW_TITLE, FW_TITLE) == 0) {
    if (strcmp(NEW_VERSION, CURRENT_VERSION) != 0) {
      // Perform actions if FW_version matches the desired version
      strcpy(CURRENT_VERSION, NEW_VERSION);
      Serial.println("\n");
      Serial.println("new FW_version available.");
      Serial.println("\n");
      handleSketchDownload(TOKEN, FW_TITLE, CURRENT_VERSION);

    } else {
      if (msg == 0) {
        Serial.println("\n");
        Serial.println("updates NOT available...");
        Serial.println("\n");
        msg = 1;
      }
    }
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