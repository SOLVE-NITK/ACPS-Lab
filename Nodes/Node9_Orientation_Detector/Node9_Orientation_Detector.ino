#include <ThingsBoard.h>
#include <WiFi.h>
#include <MPU9250.h>

// constexpr char WIFI_SSID[] PROGMEM = "NITK-NET";
// constexpr char WIFI_PASSWORD[] PROGMEM = "2K16NITK";

constexpr char WIFI_SSID[] PROGMEM = "CSD";
constexpr char WIFI_PASSWORD[] PROGMEM = "csd@NITK2014";

constexpr char TOKEN[] PROGMEM = "m314FQf8U6vq6N2sE2fE";

constexpr char THINGSBOARD_SERVER[] PROGMEM = "10.100.80.26";
constexpr uint16_t THINGSBOARD_PORT PROGMEM = 1883U;
constexpr uint32_t MAX_MESSAGE_SIZE PROGMEM = 128U;
constexpr uint32_t SERIAL_DEBUG_BAUD PROGMEM = 115200U;

IPAddress localIP(10, 100, 80, 43);
IPAddress gateway(10, 100, 80, 1);
IPAddress subnet(255, 255, 252, 0);

IPAddress dns1(10, 20, 1, 22);
IPAddress dns2(10, 3, 0, 101);

constexpr char ROLL_KEY[] PROGMEM = "roll";
constexpr char PITCH_KEY[] PROGMEM = "pitch";
constexpr char YAW_KEY[] PROGMEM = "yaw";
constexpr char IMPACT_KEY[] PROGMEM = "impact";
constexpr char CRASH_KEY[] PROGMEM = "crash";
constexpr char FALL_KEY[] PROGMEM = "fall";

constexpr const char RPC_TEMPERATURE_METHOD[] PROGMEM = "example_set_temperature";
constexpr const char RPC_SWITCH_METHOD[] PROGMEM = "example_set_switch";
constexpr const char RPC_TEMPERATURE_KEY[] PROGMEM = "temp";
constexpr const char RPC_SWITCH_KEY[] PROGMEM = "switch";
constexpr const char RPC_RESPONSE_KEY[] PROGMEM = "example_response";

WiFiClient espClient;
ThingsBoard tb(espClient, MAX_MESSAGE_SIZE);

int status = WL_IDLE_STATUS;  // the Wifi radio's status
bool subscribed = false;

MPU9250 mpu;

#define LED_PIN 19     // LED connected to digital pin D18
#define BUZZER_PIN 18  // Buzzer connected to digital pin D5

#define WIFI_STATUS_LED 2

#define BUZZER_VOLUME 10  // Adjust this value to control the buzzer BUZZER_VOLUME

#define DEBOUNCE_TIME 3000
#define BLINK_INTERVAL 250

float accelX_g, accelY_g, accelZ_g, accelMagnitude;
float roll_deg, pitch_deg, yaw_deg;

bool buzzerState = false;

/// @brief Initalizes WiFi connection,
// will endlessly delay until a connection has been successfully established
void InitWiFi() {

  WiFi.config(localIP, gateway, subnet, dns1, dns2);

  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to network: ");
    Serial.println(WIFI_SSID);
    status = WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    delay(5000);
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

/// Processes function for RPC call "example_set_temperature"
/// RPC_Data is a JSON variant, that can be queried using operator[]
/// See https://arduinojson.org/v5/api/jsonvariant/subscript/ for more details
/// "data" Data containing the rpc data that was called and its current value
///  Response that should be sent to the cloud. Useful for getMethods
RPC_Response processTemperatureChange(const RPC_Data &data) {
  Serial.println(F("Received the set temperature RPC method"));

  // Process data
  const float example_temperature = data[RPC_TEMPERATURE_KEY];

  Serial.print(F("Example temperature: "));
  Serial.println(example_temperature);

  // Just an response example
  StaticJsonDocument<JSON_OBJECT_SIZE(1)> doc;
  doc[RPC_RESPONSE_KEY] = 42;
  return RPC_Response(doc);
}

/// @brief Processes function for RPC call "example_set_switch"
/// RPC_Data is a JSON variant, that can be queried using operator[]
/// See https://arduinojson.org/v5/api/jsonvariant/subscript/ for more details
/// @param data Data containing the rpc data that was called and its current value
/// @return Response that should be sent to the cloud. Useful for getMethods
RPC_Response processSwitchChange(const RPC_Data &data) {
  Serial.println(F("Received the set switch method"));

  // Process data
  const bool switch_state = data[RPC_SWITCH_KEY];

  Serial.print(F("Example switch state: "));
  Serial.println(switch_state);

  // Just an response example
  StaticJsonDocument<JSON_OBJECT_SIZE(1)> doc;
  doc[RPC_RESPONSE_KEY] = 22.02;
  return RPC_Response(doc);
}

const std::array<RPC_Callback, 2U> callbacks = {
  RPC_Callback{ RPC_TEMPERATURE_METHOD, processTemperatureChange },
  RPC_Callback{ RPC_SWITCH_METHOD, processSwitchChange }
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
    tb.sendTelemetryString(FALL_KEY, "DETECTED!");
    tb.sendAttributeBool(IMPACT_KEY, true);
    actuate();
  } else {
    tb.sendTelemetryString(FALL_KEY, "NORMAL :)");
    tb.sendAttributeBool(IMPACT_KEY, false);
  }

  if (accelMagnitude > crashThreshold) {
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

  // Uploads new telemetry to ThingsBoard
  Serial.println(F("Sending data..."));

  tb.loop();
}
