#include <ThingsBoard.h>
#include <WiFiNINA.h>

constexpr char WIFI_SSID[] = "NITK-NET";
constexpr char WIFI_PASSWORD[] = "2K16NITK";

// constexpr char WIFI_SSID[] = "CSD";
// constexpr char WIFI_PASSWORD[] = "csd@NITK2014";

constexpr char TOKEN[] = "8ZUAtcOeTtOzaviknlfT";

constexpr char THINGSBOARD_SERVER[] = "10.100.80.26";
constexpr uint16_t THINGSBOARD_PORT = 1883U;
constexpr uint32_t MAX_MESSAGE_SIZE = 128U;
constexpr uint32_t SERIAL_DEBUG_BAUD = 115200U;

IPAddress localIP(10, 100, 80, 38);
IPAddress gateway(10, 100, 80, 1);
IPAddress subnet(255, 255, 252, 0);
IPAddress dns(0, 0, 0, 0);

IPAddress dns1(10, 20, 1, 22);
IPAddress dns2(10, 3, 0, 101);

constexpr char FIRE_KEY[] = "fire";
constexpr char SMOKE_KEY[] = "smoke";
constexpr char ACTUATOR_KEY[] PROGMEM = "actuator";

constexpr const char RPC_FIRE_METHOD[] = "example_set_fire";
constexpr const char RPC_SWITCH_METHOD[] = "example_set_switch";
constexpr const char RPC_FIRE_KEY[] = "temp";
constexpr const char RPC_SWITCH_KEY[] PROGMEM = "switch";
constexpr const char RPC_RESPONSE_KEY[] = "example_response";

// Pin definitions
#define IR_PIN 2
#define MQ11_PIN A1
#define BUZZER_PIN 5
#define LED_PIN 4

#define WIFI_STATUS_LED LED_BUILTIN

// Threshold values
#define SMOKE_THRESHOLD 200

WiFiClient espClient;
ThingsBoard tb(espClient, MAX_MESSAGE_SIZE);

int status = WL_IDLE_STATUS;  // the Wifi radio's status
bool subscribed = false;

int buzzerVolume = 5;

int actuatorState;

/// @brief Initalizes WiFi connection,
// will endlessly delay until a connection has been successfully established
void InitWiFi() {

  // Configure static IP and DNS server
  WiFi.config(localIP, dns, gateway, subnet);
  WiFi.setDNS(dns1, dns2);

  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to network: ");
    Serial.println(WIFI_SSID);
    status = WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    delay(5000);
  }
  Serial.println("Connected to AP");
}

/// @brief Reconnects the WiFi uses InitWiFi if the connection has been removed
/// @return Returns true as soon as a connection has been established again
bool reconnect() {
  // Check to ensure we aren't connected yet
  const uint8_t status = WiFi.status();
  if (status == WL_CONNECTED) {
    return true;
  }

  // If we aren't establish a new connection to the given WiFi network
  InitWiFi();
  return true;
}

/// Processes function for RPC call "example_set_fire"
/// RPC_Data is a JSON variant, that can be queried using operator[]
/// See https://arduinojson.org/v5/api/jsonvariant/subscript/ for more details
/// "data" Data containing the rpc data that was called and its current value
///  Response that should be sent to the cloud. Useful for getMethods
RPC_Response processfireChange(const RPC_Data &data) {
  Serial.println("Received the set fire RPC method");

  // Process data
  const float example_fire = data[RPC_FIRE_KEY];

  Serial.print("Example fire: ");
  Serial.println(example_fire);

  // Just an response example
  StaticJsonDocument<JSON_OBJECT_SIZE(1)> doc;
  doc[RPC_RESPONSE_KEY] = 42;
  return RPC_Response(doc);
}

/// Processes function for RPC call "example_set_switch"
/// RPC_Data is a JSON variant, that can be queried using operator[]
/// See https://arduinojson.org/v5/api/jsonvariant/subscript/ for more details
/// "data" Data containing the rpc data that was called and its current value
/// Response that should be sent to the cloud. Useful for getMethods
RPC_Response processSwitchChange(const RPC_Data &data) {
  Serial.println("Received the set switch method");

  // Process data
  const bool switch_state = data[RPC_SWITCH_KEY];

  Serial.print("Example switch state: ");
  Serial.println(switch_state);

  // Just an response example
  StaticJsonDocument<JSON_OBJECT_SIZE(1)> doc;
  doc[RPC_RESPONSE_KEY] = 22.02;
  return RPC_Response(doc);
}

const std::array<RPC_Callback, 2U> callbacks = {
  RPC_Callback{ RPC_FIRE_METHOD, processfireChange },
  RPC_Callback{ RPC_SWITCH_METHOD, processSwitchChange }
};


void activateAlarm() {
  analogWrite(BUZZER_PIN, buzzerVolume);
  digitalWrite(LED_PIN, HIGH);
  actuatorState = 1;
}

void deactivateAlarm() {
  analogWrite(BUZZER_PIN, 0);
  digitalWrite(LED_PIN, LOW);
  actuatorState = 0;
}

void setup() {
  // Initalize serial connection for debugging
  Serial.begin(SERIAL_DEBUG_BAUD);

  pinMode(IR_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(WIFI_STATUS_LED, OUTPUT);

  digitalWrite(WIFI_STATUS_LED, LOW);

  delay(1000);
  InitWiFi();
}

void loop() {
  delay(1000);

  if (!reconnect()) {
    return;
  }

  if (!tb.connected()) {
    // Reconnect to the ThingsBoard server,
    // if a connection was disrupted or has not yet been established
    Serial.print("Connecting to: ");
    Serial.print(THINGSBOARD_SERVER);
    Serial.print(" with token ");
    Serial.println(TOKEN);
    if (!tb.connect(THINGSBOARD_SERVER, TOKEN, THINGSBOARD_PORT)) {

      Serial.println("Failed to connect");
      return;
    }
  }

  if (!subscribed) {
    Serial.println("Subscribing for RPC...");

    // Perform a subscription. All consequent data processing will happen in
    // processfireChange() and processSwitchChange() functions,
    // as denoted by callbacks array.
    if (!tb.RPC_Subscribe(callbacks.cbegin(), callbacks.cend())) {
      Serial.println("Failed to subscribe for RPC");
      digitalWrite(WIFI_STATUS_LED, LOW);
      return;
    }
    Serial.println("Subscribe done");
    digitalWrite(WIFI_STATUS_LED, HIGH);

    subscribed = true;
  }

  // Read sensor values
  int irSensorValue = digitalRead(IR_PIN);
  int mq11SensorValue = analogRead(MQ11_PIN);

  // Check if flame is detected
  if (irSensorValue == LOW || mq11SensorValue >= SMOKE_THRESHOLD) {
    if (irSensorValue == LOW) {
      Serial.println("Flame detected!");
      tb.sendTelemetryString(FIRE_KEY, "Flame detected!");
    } else {
      Serial.println("Smoke detected!");
      tb.sendTelemetryString(SMOKE_KEY, "Smoke detected!");
    }
    activateAlarm();
  } else {
    deactivateAlarm();
    tb.sendTelemetryString(FIRE_KEY, "Normal");
    tb.sendTelemetryString(SMOKE_KEY, "Normal");
  }

  // Uploads new telemetry to ThingsBoard using MQTT.
  Serial.println("Sending fire data...");

  tb.sendAttributeInt(ACTUATOR_KEY, actuatorState);

  tb.loop();
}
