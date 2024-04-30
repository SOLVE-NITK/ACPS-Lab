#ifdef ESP8266
#include <ESP8266WiFi.h>
#define THINGSBOARD_ENABLE_PROGMEM 0
#endif  // ESP8266

#include <ThingsBoard.h>
#include <Servo.h>

// constexpr char WIFI_SSID[] = "NITK-NET";
// constexpr char WIFI_PASSWORD[] = "2K16NITK";

constexpr char WIFI_SSID[] = "CSD";
constexpr char WIFI_PASSWORD[] = "csd@NITK2014";

constexpr char TOKEN[] = "TMMdzP4UaqyZw4I3suXP";

constexpr char THINGSBOARD_SERVER[] = "10.100.80.26";
constexpr uint16_t THINGSBOARD_PORT = 1883U;
constexpr uint32_t MAX_MESSAGE_SIZE = 128U;
constexpr uint32_t SERIAL_DEBUG_BAUD = 115200U;

IPAddress localIP(10, 100, 80, 41);
IPAddress gateway(10, 100, 80, 1);
IPAddress subnet(255, 255, 252, 0);

IPAddress dns1(10, 20, 1, 22);
IPAddress dns2(10, 3, 0, 101);

constexpr char MOTION_KEY[] = "motion";
constexpr char ACTUATOR_KEY[] = "actuator";

constexpr const char RPC_TEMPERATURE_METHOD[] = "example_set_temperature";
constexpr const char RPC_SWITCH_METHOD[] = "example_set_switch";
constexpr const char RPC_TEMPERATURE_KEY[] = "temp";
constexpr const char RPC_SWITCH_KEY[] = "switch";
constexpr const char RPC_RESPONSE_KEY[] = "example_response";

#define LED_PIN 12
#define PIR_PIN 14
#define SERVO_PIN 13

#define WIFI_STATUS_LED LED_BUILTIN

WiFiClient espClient;
ThingsBoard tb(espClient, MAX_MESSAGE_SIZE);

Servo myservo;

int status = WL_IDLE_STATUS;  // the Wifi radio's status
bool subscribed = false;

//variables to keep track of the timing of recent interrupts
unsigned long button_time = 0;
unsigned long last_button_time = 0;

#define CLOSE_POS 180
#define OPEN_POS 40

#define DEBOUNCE_TIME 6000

bool motionDetected = false;

int pos = 0;

int actuatorState = 0;

/// @brief Initalizes WiFi connection,
// will endlessly delay until a connection has been successfully established
void InitWiFi() {

  // Configure static IP and DNS server
  WiFi.config(localIP, gateway, subnet, dns1, dns2);
  // WiFi.setDNS(dns1, dns2);

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

// Checks if motion was detected
ICACHE_RAM_ATTR void detectMotion() {
  button_time = millis();
  if (button_time - last_button_time > DEBOUNCE_TIME) {
    motionDetected = true;
    last_button_time = button_time;
  }
}

void openDoor() {
  for (pos = CLOSE_POS; pos >= OPEN_POS; pos -= 10) {
    myservo.write(pos);
    delay(1);
  }
}

void closeDoor() {
  for (pos = OPEN_POS; pos <= CLOSE_POS; pos += 10) {
    myservo.write(pos);
    delay(1);
  }
}

void setup() {
  // Initalize serial connection for debugging
  Serial.begin(SERIAL_DEBUG_BAUD);

  pinMode(PIR_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(PIR_PIN), detectMotion, RISING);

  myservo.attach(SERVO_PIN);
  pinMode(LED_PIN, OUTPUT);

  digitalWrite(LED_PIN, LOW);
  myservo.write(CLOSE_POS);

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
    Serial.printf("Connecting to: (%s) with token (%s)\n", THINGSBOARD_SERVER, TOKEN);
    digitalWrite(WIFI_STATUS_LED, LOW);
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
      return;
    }
    Serial.println("Subscribe done");
    digitalWrite(WIFI_STATUS_LED, HIGH);

    subscribed = true;
  }

  if (motionDetected) {
    Serial.println("Motion Detected!");
    digitalWrite(LED_PIN, HIGH);
    actuatorState = 1;
    tb.sendAttributeBool(MOTION_KEY, motionDetected);
    tb.sendTelemetryString(ACTUATOR_KEY, "Door is Open");
    openDoor();
    delay(3000);
    closeDoor();
    motionDetected = false;
  } else {
    digitalWrite(LED_PIN, LOW);
    myservo.write(CLOSE_POS);
    actuatorState = 0;
    tb.sendAttributeBool(MOTION_KEY, motionDetected);
    tb.sendTelemetryString(ACTUATOR_KEY, "Door is Close");
  }

  // Uploads new telemetry to ThingsBoard using MQTT.
  Serial.println("Sending motion detection data...");


  tb.loop();
}
