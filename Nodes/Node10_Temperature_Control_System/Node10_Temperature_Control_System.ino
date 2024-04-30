#include <ThingsBoard.h>
#include <WiFiNINA.h>
#include <LM35.h>

// constexpr char WIFI_SSID[] = "NITK-NET";
// constexpr char WIFI_PASSWORD[] = "2K16NITK";

constexpr char WIFI_SSID[] = "CSD";
constexpr char WIFI_PASSWORD[] = "csd@NITK2014";

constexpr char TOKEN[] = "oMAB7O8B0LkQvHkzSxvY";

constexpr char THINGSBOARD_SERVER[] = "10.100.80.26";
constexpr uint16_t THINGSBOARD_PORT = 1883U;
constexpr uint32_t MAX_MESSAGE_SIZE = 128U;
constexpr uint32_t SERIAL_DEBUG_BAUD = 115200U;

IPAddress localIP(10, 100, 80, 44);
IPAddress gateway(10, 100, 80, 1);
IPAddress subnet(255, 255, 252, 0);
IPAddress dns(0, 0, 0, 0);

IPAddress dns1(10, 20, 1, 22);
IPAddress dns2(10, 3, 0, 101);

constexpr char TEMPERATURE_KEY[] = "temperature";
constexpr char ACTUATOR_KEY[] = "actuatorState";

constexpr const char RPC_TEMPERATURE_METHOD[] = "example_set_temperature";
constexpr const char RPC_SWITCH_METHOD[] = "example_set_switch";
constexpr const char RPC_TEMPERATURE_KEY[] = "temp";
constexpr const char RPC_SWITCH_KEY[] PROGMEM = "switch";
constexpr const char RPC_RESPONSE_KEY[] = "example_response";

#define LM35_PIN A1 // Analog pin A0 for LM35
#define LED_PIN 7  // Digital pin for LED
#define FAN_PIN 8  // Digital pin for Fan

#define WIFI_STATUS_LED LED_BUILTIN

WiFiClient espClient;
ThingsBoard tb(espClient, MAX_MESSAGE_SIZE);

LM35 sensor(LM35_PIN);

int status = WL_IDLE_STATUS;  // the Wifi radio's status
bool subscribed = false;

#define TEMP_THRESHOLD 45

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
    delay(2000);
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

/// Processes function for RPC call "example_set_temperature"
/// RPC_Data is a JSON variant, that can be queried using operator[]
/// See https://arduinojson.org/v5/api/jsonvariant/subscript/ for more details
/// "data" Data containing the rpc data that was called and its current value
///  Response that should be sent to the cloud. Useful for getMethods
RPC_Response processTemperatureChange(const RPC_Data &data) {
  Serial.println("Received the set temperature RPC method");

  // Process data
  const float example_temperature = data[RPC_TEMPERATURE_KEY];

  Serial.print("Example temperature: ");
  Serial.println(example_temperature);

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
  RPC_Callback{ RPC_TEMPERATURE_METHOD, processTemperatureChange },
  RPC_Callback{ RPC_SWITCH_METHOD, processSwitchChange }
};

void setup() {
  // Initalize serial connection for debugging
  Serial.begin(SERIAL_DEBUG_BAUD);
  delay(1000);

  pinMode(LED_PIN, OUTPUT);  // Set LED pin as output
  pinMode(FAN_PIN, OUTPUT);  // Set fan pin as output
  pinMode(WIFI_STATUS_LED, OUTPUT);

  InitWiFi();

  digitalWrite(LED_PIN, LOW);  // Turn off the LED
  digitalWrite(FAN_PIN, LOW);  // Turn off the Fan
  digitalWrite(WIFI_STATUS_LED, LOW);

  actuatorState = 0;
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
    // processTemperatureChange() and processSwitchChange() functions,
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

  int temperature = sensor.getTemp();  // Read temperature from LM35 sensor
  Serial.print("Temperature: ");
  Serial.println(temperature);

  if (temperature > TEMP_THRESHOLD) {
    digitalWrite(LED_PIN, HIGH);  // Turn on the LED
    digitalWrite(FAN_PIN, HIGH);  // Turn on the Fan
    actuatorState = 1;
  } else {
    digitalWrite(LED_PIN, LOW);  // Turn off the LED
    digitalWrite(FAN_PIN, LOW);  // Turn off the Fan
    actuatorState = 0;
  }

  // Uploads new telemetry to ThingsBoard using MQTT.
  Serial.println("Sending data...");
  tb.sendTelemetryInt(TEMPERATURE_KEY, temperature);
  tb.sendAttributeBool(ACTUATOR_KEY, actuatorState);

  tb.loop();
}
