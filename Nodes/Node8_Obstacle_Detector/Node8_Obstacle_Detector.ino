#include <ThingsBoard.h>
#include <WiFi.h>
#include <NewPing.h>

// constexpr char WIFI_SSID[] PROGMEM = "NITK-NET";
// constexpr char WIFI_PASSWORD[] PROGMEM = "2K16NITK";

constexpr char WIFI_SSID[] PROGMEM = "CSD";
constexpr char WIFI_PASSWORD[] PROGMEM = "csd@NITK2014";

constexpr char TOKEN[] PROGMEM = "nOqz1O7R0S1NnLq42i7T";

constexpr char THINGSBOARD_SERVER[] PROGMEM = "10.100.80.26";
constexpr uint16_t THINGSBOARD_PORT PROGMEM = 1883U;
constexpr uint32_t MAX_MESSAGE_SIZE PROGMEM = 128U;
constexpr uint32_t SERIAL_DEBUG_BAUD PROGMEM = 115200U;

IPAddress localIP(10, 100, 80, 42);
IPAddress gateway(10, 100, 80, 1);
IPAddress subnet(255, 255, 252, 0);

IPAddress dns1(10, 20, 1, 22);
IPAddress dns2(10, 3, 0, 101);

constexpr char DISTANCE_KEY[] PROGMEM = "distance";
constexpr char ACTUATOR_KEY[] PROGMEM = "actuator";

constexpr const char RPC_TEMPERATURE_METHOD[] PROGMEM = "example_set_temperature";
constexpr const char RPC_SWITCH_METHOD[] PROGMEM = "example_set_switch";
constexpr const char RPC_TEMPERATURE_KEY[] PROGMEM = "temp";
constexpr const char RPC_SWITCH_KEY[] PROGMEM = "switch";
constexpr const char RPC_RESPONSE_KEY[] PROGMEM = "example_response";

WiFiClient espClient;
ThingsBoard tb(espClient, MAX_MESSAGE_SIZE);

int status = WL_IDLE_STATUS;  // the Wifi radio's status
bool subscribed = false;

#define TRIG_PIN 5    // ESP32 pin GIOP26 connected to Ultrasonic Sensor's TRIG pin
#define ECHO_PIN 18    // ESP32 pin GIOP25 connected to Ultrasonic Sensor's ECHO pin
#define BUZZER_PIN 19  // Buzzer connected to digital pin D5
#define LED_PIN 21     // ESP32 pin GIOP17 connected to LED's pin

#define WIFI_STATUS_LED 2

#define MAX_DISTANCE 400
#define DISTANCE_THRESHOLD 10  // centimeters

#define BUZZER_VOLUME 10  // Adjust this value to control the buzzer BUZZER_VOLUME

NewPing sonar(TRIG_PIN, ECHO_PIN, MAX_DISTANCE);  // NewPing setup of pins and maximum distance.

int distance;

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

int getDistance() {
  delay(50);
  return sonar.ping_cm();
}

void setup() {
  // Initalize serial connection for debugging
  Serial.begin(SERIAL_DEBUG_BAUD);

  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(WIFI_STATUS_LED, OUTPUT);

  digitalWrite(LED_PIN, LOW);
  analogWrite(BUZZER_PIN, 0);
  digitalWrite(WIFI_STATUS_LED, LOW);

  delay(1000);
  InitWiFi();
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

  distance = getDistance();

  tb.sendTelemetryInt(DISTANCE_KEY, distance);

  Serial.print("Distance = ");
  Serial.println(distance);

  if (distance < DISTANCE_THRESHOLD) {
    Serial.println("Obstacle Detected!");
    tb.sendAttributeBool(ACTUATOR_KEY, true);
    digitalWrite(LED_PIN, HIGH);
    analogWrite(BUZZER_PIN, BUZZER_VOLUME);
  } else {
    tb.sendAttributeBool(ACTUATOR_KEY, false);
    digitalWrite(LED_PIN, LOW);
    analogWrite(BUZZER_PIN, 0);
  }

  // Uploads new telemetry to ThingsBoard
  Serial.println(F("Sending data..."));

  tb.loop();
}