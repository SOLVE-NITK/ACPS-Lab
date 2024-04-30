#include <ThingsBoard.h>
#include <WiFi.h>

constexpr char WIFI_SSID[] = "CSD";
constexpr char WIFI_PASSWORD[] = "csd@NITK2014";

constexpr char TOKEN[] = "nmiEJMmAf1xhG30cT6cu";

constexpr char THINGSBOARD_SERVER[] = "10.100.80.26";
constexpr uint16_t THINGSBOARD_PORT = 1883U;
constexpr uint32_t MAX_MESSAGE_SIZE = 128U;
constexpr uint32_t SERIAL_DEBUG_BAUD = 115200U;

IPAddress localIP(10, 100, 80, 45);
IPAddress gateway(10, 100, 80, 1);
IPAddress subnet(255, 255, 252, 0);
IPAddress dns(0, 0, 0, 0);

IPAddress dns1(10, 20, 1, 22);
IPAddress dns2(10, 3, 0, 101);

constexpr char VIBRATION_KEY[] = "vibration";
constexpr char ACTUATOR_KEY[] = "actuator";

constexpr const char RPC_Vibration_METHOD[] = "example_set_Vibration";
constexpr const char RPC_SWITCH_METHOD[] = "example_set_switch";
constexpr const char RPC_Vibration_KEY[] = "temp";
constexpr const char RPC_SWITCH_KEY[] = "switch";
constexpr const char RPC_RESPONSE_KEY[] = "example_response";

WiFiClient espClient;
ThingsBoard tb(espClient, MAX_MESSAGE_SIZE);

int status = WL_IDLE_STATUS;  // the Wifi radio's status
bool subscribed = false;

#define VIBRATION_SENSOR 2
#define LED_PIN 4
#define BUZZER_PIN 5

#define WIFI_STATUS_LED LED_BUILTIN

#define BUZZER_VOLUME 10

#define DEBOUNCE_TIME 3000
#define BLINK_INTERVAL 250

//variables to keep track of the timing of recent interrupts
unsigned long button_time = 0;
unsigned long last_button_time = 0;

bool vibrationDetected = false, buzzerState = false;

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

/// Processes function for RPC call "example_set_Vibration"
/// RPC_Data is a JSON variant, that can be queried using operator[]
/// See https://arduinojson.org/v5/api/jsonvariant/subscript/ for more details
/// "data" Data containing the rpc data that was called and its current value
///  Response that should be sent to the cloud. Useful for getMethods
RPC_Response processVibrationChange(const RPC_Data &data) {
  Serial.println("Received the set Vibration RPC method");

  // Process data
  const float example_Vibration = data[RPC_Vibration_KEY];

  Serial.print("Example Vibration: ");
  Serial.println(example_Vibration);

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
  RPC_Callback{ RPC_Vibration_METHOD, processVibrationChange },
  RPC_Callback{ RPC_SWITCH_METHOD, processSwitchChange }
};

void detectVibration() {
  button_time = millis();
  if (button_time - last_button_time > DEBOUNCE_TIME) {
    vibrationDetected = true;
    last_button_time = button_time;
  }
}

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
  delay(1000);

  pinMode(VIBRATION_SENSOR, INPUT);
  attachInterrupt(VIBRATION_SENSOR, detectVibration, RISING);

  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(WIFI_STATUS_LED, OUTPUT);

  digitalWrite(LED_PIN, LOW);
  analogWrite(BUZZER_PIN, 0);
  digitalWrite(WIFI_STATUS_LED, LOW);

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
    Serial.printf("Connecting to: (%s) with token (%s)\n", THINGSBOARD_SERVER, TOKEN);
    if (!tb.connect(THINGSBOARD_SERVER, TOKEN, THINGSBOARD_PORT)) {
      Serial.println("Failed to connect");
      return;
    }
  }

  if (!subscribed) {
    Serial.println("Subscribing for RPC...");

    // Perform a subscription. All consequent data processing will happen in
    // processVibrationChange() and processSwitchChange() functions,
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

  if (vibrationDetected) {
    tb.sendTelemetryString(VIBRATION_KEY, "Shock Detected!");
    tb.sendAttributeBool(ACTUATOR_KEY, true);
    actuate();
    vibrationDetected = false;
  } else {
    tb.sendTelemetryString(VIBRATION_KEY, "Normal");
    tb.sendAttributeBool(ACTUATOR_KEY, false);
    digitalWrite(LED_PIN, LOW);
    analogWrite(BUZZER_PIN, 0);
  }

  // Uploads new telemetry to ThingsBoard using MQTT.
  Serial.println("Sending Vibration data...");

  tb.loop();
}