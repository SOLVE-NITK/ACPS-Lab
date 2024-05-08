#ifdef ESP8266
#include <ESP8266WiFi.h>
#define THINGSBOARD_ENABLE_PROGMEM 0
#endif  // ESP8266
#include <ThingsBoard.h>
#include <Servo.h>

#define LED_PIN 12
#define PIR_PIN 14
#define SERVO_PIN 13
#define WIFI_STATUS_LED LED_BUILTIN
#define CLOSE_POS 180
#define OPEN_POS 40
#define DEBOUNCE_TIME 6000

constexpr char WIFI_SSID[] = "CSD";
constexpr char WIFI_PASSWORD[] = "csd@NITK2014";
constexpr char TOKEN[] = "Ct0199UNxnMBtfUeSpLE";
constexpr char THINGSBOARD_SERVER[] = "10.14.0.205";
constexpr uint16_t THINGSBOARD_PORT = 1883U;
constexpr uint32_t MAX_MESSAGE_SIZE = 128U;
constexpr uint32_t SERIAL_DEBUG_BAUD = 115200U;

constexpr char MOTION_KEY[] = "motion";
constexpr char ACTUATOR_KEY[] = "actuator";
constexpr const char MOTION_TELEMETRY[] = "telemetry_data";
constexpr const char RPC_SWITCH_METHOD[] = "openDoor";
constexpr const char RPC_TEMPERATURE_KEY[] = "temp";
constexpr const char RPC_SWITCH_KEY[] = "openDoor";
constexpr const char RPC_RESPONSE_KEY[] = "example_response";
int status = WL_IDLE_STATUS;  // the Wifi radio's status
bool subscribed = false;
//variables to keep track of the timing of recent interrupts
unsigned long button_time = 0;
unsigned long last_button_time = 0;
bool motionDetected = false;
int pos = 0;
int actuatorState = 0;

int switch_state;

unsigned long switchTimer = 0;
const int switchDuration = 7000;  // 5 seconds in milliseconds

unsigned long previousMillis = 0;  // will store last time LED was updated

// constants won't change:
const long interval = 2000;  // interval at which to blink (milliseconds)

Servo myservo;  //object  created for servo

WiFiClient espClient;
ThingsBoard tb(espClient, MAX_MESSAGE_SIZE);

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
RPC_Response setServoSwitchState(RPC_Data &data) {
  Serial.println("RECIEVED SWITCH STATE");
  switch_state = data;
  Serial.println("SWITCH STATE CHANGE:");
  Serial.print(switch_state);
  return RPC_Response("setServoSwitchValue", switch_state);
}

const std::array<RPC_Callback, 2U> callbacks = {
  RPC_Callback{ "setServoSwitchValue", setServoSwitchState },
  RPC_Callback{ RPC_SWITCH_METHOD, setServoSwitchState }
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
  if (myservo.read() != OPEN_POS) {
    for (pos = CLOSE_POS; pos >= OPEN_POS; pos -= 10) {
      myservo.write(pos);
      delay(1);
    }
  }
}

void closeDoor() {
  if (myservo.read() != CLOSE_POS) {
    for (pos = OPEN_POS; pos <= CLOSE_POS; pos += 10) {
      myservo.write(pos);
      delay(1);
    }
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

  unsigned long currentMillis = millis();
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

  tb.sendTelemetryBool(MOTION_KEY, motionDetected);

  Serial.println();
  Serial.print(motionDetected);
  Serial.print("\t");
  Serial.println(digitalRead(PIR_PIN));

  // Check if switch_state is 40
  if (switch_state == 40) {
    // Check if the switch timer is not running
    if (switchTimer == 0) {
      digitalWrite(LED_PIN, HIGH);  // Turn on the LED
      tb.sendTelemetryString(MOTION_TELEMETRY, "Door is open");
      tb.sendAttributeBool(ACTUATOR_KEY, true);
      openDoor();  // Open the door
      // Start the timer
      switchTimer = millis();  // Record the current time
    }
  } else {
    digitalWrite(LED_PIN, LOW);  // Turn off the LED
    tb.sendTelemetryString(MOTION_TELEMETRY, "Door is closed");
    tb.sendAttributeBool(ACTUATOR_KEY, false);
    closeDoor();  // Close the door
  }

  // Reset motionDetected flag
  motionDetected = false;

  // Check if 5 seconds have passed since the switch was activated
  if (switchTimer != 0 && millis() - switchTimer >= switchDuration) {
    // Reset switch state and timer
    switch_state = 0;  // Reset switch state
    switchTimer = 0;   // Reset the switch timer
  }
  tb.loop();
}