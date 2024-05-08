#include <ThingsBoard.h>
#include <WiFiNINA.h>
#include <LM35.h>
#include <WiFi.h>

#define LM35_PIN A1  // Analog pin A0 for LM35
#define LED_PIN 7    // Digital pin for LED
#define FAN_PIN 8    // Digital pin for Fan

#define TEMP_THRESHOLD 43

#define WIFI_STATUS_LED LED_BUILTIN

// constexpr char WIFI_SSID[] = "NITK-NET";
// constexpr char WIFI_PASSWORD[] = "2K16NITK";

constexpr char WIFI_SSID[] = "CSD";
constexpr char WIFI_PASSWORD[] = "csd@NITK2014";

constexpr char TOKEN[] = "WozbMrsNP48z2dEtAn29";

constexpr char THINGSBOARD_SERVER[] = "10.14.0.205";
constexpr uint16_t THINGSBOARD_PORT = 1883U;
constexpr uint32_t MAX_MESSAGE_SIZE = 128U;
constexpr uint32_t SERIAL_DEBUG_BAUD = 115200U;

constexpr char TEMPERATURE_KEY[] = "temperature";
constexpr char ACTUATOR_KEY[] = "actuatorState";

constexpr const char TEMPERATURE_TELEMETRY[] = "temperature_data";
constexpr const char RPC_SWITCH_METHOD[] = "temp";
constexpr const char RPC_TEMPERATURE_KEY[] = "temp";
constexpr const char RPC_SWITCH_KEY[] PROGMEM = "switch";
constexpr const char RPC_RESPONSE_KEY[] = "example_response";

WiFiClient espClient;
ThingsBoard tb(espClient, MAX_MESSAGE_SIZE);

LM35 sensor(LM35_PIN);

int status = WL_IDLE_STATUS;  // the Wifi radio's status
bool subscribed = false;

unsigned long startTime;
bool runningCode = false;

int switch_state;

/// @brief Initalizes WiFi connection,
// will endlessly delay until a connection has been successfully established
void InitWiFi() {
  Serial.begin(115200);
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(WIFI_SSID);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    delay(5000);  // Wait for 5 seconds before checking connection status
    if (attempts >= 10) {
      Serial.println("Connection failed after 10 attempts.");
      return;  // Exit the function if connection fails after 10 attempts
    }
    attempts++;
  }
  Serial.println("Connected to WiFi");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
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
RPC_Response setServoSwitchState(RPC_Data &data) {
  Serial.println("RECIEVED SWITCH STATE");
  switch_state = data;
  Serial.println("SWITCH STATE CHANGE:");
  Serial.print(switch_state);
  return RPC_Response("actuatorState", switch_state);
}

const std::array<RPC_Callback, 1U> callbacks = {
  RPC_Callback{ RPC_SWITCH_METHOD, setServoSwitchState }
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

  // actuatorState = 0;
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


  // Check if switch_state is 30
  if (switch_state == 30) {
    switch_state = 0;
    startTime = millis();  // Record the start time
    runningCode = true;    // Set flag to indicate code is running
  }

  // Run the code for 4 seconds if the flag is set
  if (runningCode) {
    // Run your code here
    digitalWrite(LED_PIN, HIGH);  // Turn on the LED
    digitalWrite(FAN_PIN, LOW);   // Turn on the Fan
    tb.sendTelemetryString(TEMPERATURE_TELEMETRY, "Temperature Upnormal");
    tb.sendAttributeBool(ACTUATOR_KEY, true);

    // Check if 4 seconds have elapsed
    if (millis() - startTime >= 4000) {
      // Reset the flag and turn off the LED and Fan after 4 seconds
      runningCode = false;
      digitalWrite(LED_PIN, LOW);  // Turn off the LED
      digitalWrite(FAN_PIN, LOW);  // Turn off the Fan
    }
  } else {
    // Code for the else block
    tb.sendTelemetryString(TEMPERATURE_TELEMETRY, "Temperature Normal");
    tb.sendAttributeBool(ACTUATOR_KEY, false);
    digitalWrite(LED_PIN, LOW);  // Turn off the LED
    digitalWrite(FAN_PIN, LOW);  // Turn off the Fan
  }

  // Uploads new telemetry to ThingsBoard using MQTT.
  // Serial.println("Sending data...");
  tb.sendTelemetryInt(TEMPERATURE_KEY, temperature);
  // tb.sendAttributeBool(ACTUATOR_KEY, actuatorState);

  tb.loop();
}