#include <ThingsBoard.h>
#include <WiFi.h>
#include <MPU9250.h>

#define LED_PIN 19     // LED connected to digital pin D18
#define BUZZER_PIN 18  // Buzzer connected to digital pin D5

#define WIFI_STATUS_LED 2

#define BUZZER_VOLUME 0  // Adjust this value to control the buzzer BUZZER_VOLUME

#define DEBOUNCE_TIME 3000
#define BLINK_INTERVAL 250

int CrashState = 0, FreefallState = 0;

constexpr char WIFI_SSID[] PROGMEM = "CSD";
constexpr char WIFI_PASSWORD[] PROGMEM = "csd@NITK2014";

constexpr char TOKEN[] PROGMEM = "GiHzHre1VhdFjUggkhFj";

constexpr char THINGSBOARD_SERVER[] PROGMEM = "10.14.0.205";
constexpr uint16_t THINGSBOARD_PORT PROGMEM = 1883U;
constexpr uint32_t MAX_MESSAGE_SIZE PROGMEM = 128U;
constexpr uint32_t SERIAL_DEBUG_BAUD PROGMEM = 115200U;

constexpr char ROLL_KEY[] PROGMEM = "roll";
constexpr char PITCH_KEY[] PROGMEM = "pitch";
constexpr char YAW_KEY[] PROGMEM = "yaw";
constexpr char IMPACT_KEY[] PROGMEM = "impact";
constexpr char CRASH_KEY[] PROGMEM = "crash";
constexpr char FALL_KEY[] PROGMEM = "fall";

constexpr const char RPC_SENSOR_DATA[] PROGMEM = "accelMagnitude";
constexpr const char RPC_CRASH_METHOD[] PROGMEM = "crash";
constexpr const char RPC_FREEFALL_METHOD[] PROGMEM = "freefall";
constexpr const char RPC_SWITCH_KEY[] PROGMEM = "switch";
constexpr const char RPC_RESPONSE_KEY[] PROGMEM = "example_response";

WiFiClient espClient;
ThingsBoard tb(espClient, MAX_MESSAGE_SIZE);

int status = WL_IDLE_STATUS;  // the Wifi radio's status
bool subscribed = false;

MPU9250 mpu;

float accelX_g, accelY_g, accelZ_g, accelMagnitude;
float roll_deg, pitch_deg, yaw_deg;

bool buzzerState = false;

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

RPC_Response processCrashState(RPC_Data &data) {
  Serial.println("RECIEVED CRASH STATE");
  CrashState = data;
  Serial.println("CRASH STATE CHANGE:");
  Serial.print(CrashState);
  return RPC_Response("actuatorState", CrashState);
}

RPC_Response processFreefallState(RPC_Data &data) {
  Serial.println("RECIEVED FREEFALL STATE");
  FreefallState = data;
  Serial.println("FREEFALL STATE CHANGE:");
  Serial.print(FreefallState);
  return RPC_Response("actuatorState", FreefallState);
}

const std::array<RPC_Callback, 2U> callbacks = {
  RPC_Callback{ RPC_CRASH_METHOD, processCrashState },
  RPC_Callback{ RPC_FREEFALL_METHOD, processFreefallState }
};

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
  // Initalize serial connection for debugging
  Serial.begin(SERIAL_DEBUG_BAUD);
  Wire.begin();
  delay(1000);
  InitWiFi();

  if (!mpu.setup(0x68)) {  // change to your own address
    while (1) {
      Serial.println("MPU connection failed. Please check your connection with `connection_check` example.");
      delay(5000);
    }
  }

  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(WIFI_STATUS_LED, OUTPUT);


  digitalWrite(LED_PIN, LOW);
  analogWrite(BUZZER_PIN, 0);
  digitalWrite(WIFI_STATUS_LED, LOW);
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

  if (mpu.update()) {
    // Read accelerometer data
    accelX_g = mpu.getAccX();
    accelY_g = mpu.getAccY();
    accelZ_g = mpu.getAccZ();
    accelMagnitude = sqrt(accelX_g * accelX_g + accelY_g * accelY_g + accelZ_g * accelZ_g);
  }

  // Adjust the free fall and crash threshold values based on your requirements
  float freeFallThreshold = 0.2;  // You may need to fine-tune this value
  float crashThreshold = 2.0;     // You may need to fine-tune this value
                                  //if (accelMagnitude < freeFallThreshold) {
  if (FreefallState == 40) {
    FreefallState = 0;
    Serial.println("Free fall detected!");
    tb.sendTelemetryString(FALL_KEY, "DETECTED!");
    tb.sendAttributeBool(IMPACT_KEY, true);
    actuate();
  } else {
    tb.sendTelemetryString(FALL_KEY, "NORMAL :)");
    tb.sendAttributeBool(IMPACT_KEY, false);
  }

  if (CrashState == 40) {
    CrashState = 0;
    Serial.println("Crash detected!");
    tb.sendTelemetryString(CRASH_KEY, "DETECTED!");
    tb.sendAttributeBool(IMPACT_KEY, true);
    actuate();
  } else {
    tb.sendTelemetryString(CRASH_KEY, "NORMAL :)");
    tb.sendAttributeBool(IMPACT_KEY, false);
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

    tb.sendTelemetryFloat(ROLL_KEY, roll_deg);
    tb.sendTelemetryFloat(PITCH_KEY, pitch_deg);
    tb.sendTelemetryFloat(YAW_KEY, yaw_deg);
  }

  tb.sendTelemetryData(RPC_SENSOR_DATA, accelMagnitude);

  // Uploads new telemetry to ThingsBoard
  // Serial.println(F("Sending data..."));

  tb.loop();
}