#include <ThingsBoard.h>
#include <WiFiNINA.h>
#include <WiFi.h>

#include <ArduinoOTA.h>
#include <ArduinoHttpClient.h>

// Pin definitions
#define IR_PIN 2
#define MQ11_PIN A1
#define BUZZER_PIN 5
#define LED_PIN 4

#define WIFI_STATUS_LED LED_BUILTIN

// Threshold values
#define SMOKE_THRESHOLD 200

#define DEBOUNCE_TIME 3000
#define BLINK_INTERVAL 250

constexpr char WIFI_SSID[] = "CSD";
constexpr char WIFI_PASSWORD[] = "csd@NITK2014";

constexpr char TOKEN[] = "3bKA7viQgnH8IXTWL2TT";

constexpr char THINGSBOARD_SERVER[] = "10.14.0.205";
constexpr uint16_t THINGSBOARD_PORT = 1883U;
constexpr uint32_t MAX_MESSAGE_SIZE = 256U;
constexpr uint32_t SERIAL_DEBUG_BAUD = 115200U;

constexpr char FIRE_KEY[] = "fire";
constexpr char SMOKE_KEY[] = "smoke";
constexpr char ACTUATOR_KEY[] PROGMEM = "actuator";

constexpr const char RPC_FIRE_SENSOR_DATA[] = "irSensorValue";
constexpr const char RPC_SMOKE_SENSOR_DATA[] = "mq11SensorValue";
constexpr const char RPC_FIRE_METHOD[] = "fire";
constexpr const char RPC_SMOKE_METHOD[] = "smoke";

int status = WL_IDLE_STATUS;  // the Wifi radio's status
bool subscribed = false;
int buzzerVolume = 5;
int actuatorState;
int FireState = 0, SmokeState = 0;
bool buzzerState = false;

bool requestedShared = false;
int msg = 0;

char *BASE_URL = "/api/v1";   // Define base URL for API requests
char *ENDPOINT = "firmware";  // Define endpoint for firmware updates
char PATH[256];               // Define array to store the path for firmware updates

constexpr const char TEST_KEY[] PROGMEM = "test";
constexpr const char FW_TAG_KEY[] PROGMEM = "target_fw_ts";
constexpr const char FW_CHKS_KEY[] PROGMEM = "fw_checksum";
constexpr const char FW_CHKS_ALGO_KEY[] PROGMEM = "fw_checksum_algorithm";
constexpr const char FW_SIZE_KEY[] PROGMEM = "fw_tag";
constexpr const char FW_TITLE_KEY[] PROGMEM = "fw_title";
constexpr const char FW_VER_KEY[] PROGMEM = "fw_version";

char CURRENT_VERSION[] = "1.0.3";
constexpr int FIRMWARE_SIZE = 20;           // Adjust the size according to your requirements
char NEW_VERSION[FIRMWARE_SIZE] = "1.0.3";  // Declare NEW_VERSION array

char FW_TITLE[] = "MKRFS";
constexpr int TITLE_SIZE = 20;          // Adjust the size according to your requirements
char FWW_TITLE[TITLE_SIZE] = "MKRFS";  // Declare NEW_VERSION array

// Shared attributes we want to request from the server
constexpr std::array<const char *, 6U> REQUESTED_SHARED_ATTRIBUTES = {
  FW_CHKS_KEY,
  FW_CHKS_ALGO_KEY,
  FW_SIZE_KEY,
  FW_TAG_KEY,
  FW_TITLE_KEY,
  FW_VER_KEY
};

WiFiClient wifiClient;
ThingsBoard tb(wifiClient, MAX_MESSAGE_SIZE);

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

void processSharedAttributeRequest(const Shared_Attribute_Data &data) {
  for (auto it = data.begin(); it != data.end(); ++it) {
    Serial.println(it->key().c_str());
    // Shared attributes have to be parsed by their type.
    Serial.println(it->value().as<const char *>());
    if (strcmp_P(it->key().c_str(), FW_VER_KEY) == 0) {
      // If the key is "CURRENT_VERSION", print its value
      Serial.print("NEW_VERSION: ");
      // Copy the value to NEW_VERSION array
      strncpy(NEW_VERSION, it->value().as<const char *>(), FIRMWARE_SIZE - 1);
      // Ensure null termination
      NEW_VERSION[FIRMWARE_SIZE - 1] = '\0';
      // Print the value
      Serial.println(NEW_VERSION);
    }
    if (strcmp_P(it->key().c_str(), FW_TITLE_KEY) == 0) {
      // If the key is "CURRENT_VERSION", print its value
      Serial.print("FWW_TITLE: ");
      // Copy the value to NEW_VERSION array
      strncpy(FWW_TITLE, it->value().as<const char *>(), TITLE_SIZE - 1);
      // Ensure null termination
      FWW_TITLE[TITLE_SIZE - 1] = '\0';
      // Print the value
      Serial.println(FWW_TITLE);
    }
  }

  int jsonSize = JSON_STRING_SIZE(measureJson(data));
  char buffer[jsonSize];
  serializeJson(data, buffer, jsonSize);
  Serial.println(buffer);
}

const Attribute_Request_Callback sharedCallback(REQUESTED_SHARED_ATTRIBUTES.cbegin(), REQUESTED_SHARED_ATTRIBUTES.cend(), &processSharedAttributeRequest);

void handleSketchDownload() {
  sprintf(PATH, "%s/%s/%s?title=%s&version=%s", BASE_URL, TOKEN, ENDPOINT, FW_TITLE, CURRENT_VERSION);
  const unsigned short SERVER_PORT = 8090U;                        // Commonly 80 (HTTP) | 443 (HTTPS)
  HttpClient client(wifiClient, THINGSBOARD_SERVER, SERVER_PORT);  // HTTP
  // HttpClient client(wifiClientSSL, SERVER, SERVER_PORT);  // HTTPS
  char buff[66];                           //enough size should be given to the buffer otherwise it won't get updated
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

RPC_Response processSmokeState(RPC_Data &data) {
  Serial.println("RECIEVED SMOKE STATE");
  SmokeState = data;
  Serial.println("SMOKE STATE CHANGE:");
  Serial.print(SmokeState);
  return RPC_Response("actuatorState", SmokeState);
}

RPC_Response processFireState(RPC_Data &data) {
  Serial.println("RECIEVED FIRE STATE");
  FireState = data;
  Serial.println("FIRE STATE CHANGE:");
  Serial.print(FireState);
  return RPC_Response("actuatorState", FireState);
}

const std::array<RPC_Callback, 2U> callbacks = {
  RPC_Callback{ RPC_SMOKE_METHOD, processSmokeState },
  RPC_Callback{ RPC_FIRE_METHOD, processFireState }
};


void activateAlarm() {
  unsigned long startMillis = millis();
  unsigned long previousMillis = millis();

  unsigned long interval = BLINK_INTERVAL;
  unsigned long duration = DEBOUNCE_TIME;

  while (previousMillis - startMillis < duration) {
    if (millis() - previousMillis >= interval) {
      previousMillis = millis();
      digitalWrite(LED_PIN, !digitalRead(LED_PIN));
      if (buzzerState == false) {
        analogWrite(BUZZER_PIN, buzzerVolume);
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
      handleSketchDownload();

    } else {
      if (msg == 0) {
        Serial.println("\n");
        Serial.println("updates NOT available...");
        Serial.println("\n");
        msg = 1;
      }
    }
  }

  // Read sensor values
  int irSensorValue = digitalRead(IR_PIN);
  int mq11SensorValue = analogRead(MQ11_PIN);

  tb.sendTelemetryData(RPC_SMOKE_SENSOR_DATA, mq11SensorValue);  //smoke sensor data
  tb.sendTelemetryData(RPC_FIRE_SENSOR_DATA, irSensorValue);     // fire sensor data

  if (FireState == 40) {
    FireState = 0;
    activateAlarm();
    tb.sendAttributeBool(ACTUATOR_KEY, true);
    Serial.println("Flame detected!");
    tb.sendTelemetryString(FIRE_KEY, "Flame detected!");
  }

  else if (SmokeState == 40) {
    SmokeState = 0;
    activateAlarm();
    tb.sendAttributeBool(ACTUATOR_KEY, true);
    Serial.println("Smoke detected!");
    tb.sendTelemetryString(SMOKE_KEY, "Smoke detected!");
  }

  else {
    tb.sendTelemetryString(SMOKE_KEY, "Smoke not detected!");
    tb.sendTelemetryString(FIRE_KEY, "Flame not detected!");
    tb.sendAttributeBool(ACTUATOR_KEY, false);
  }

  // Uploads new telemetry to ThingsBoard using MQTT.
  // Serial.println("Sending data...");
  Serial.println("\n");
  Serial.println(mq11SensorValue);
  Serial.println(irSensorValue);
  Serial.println("\n");

  tb.loop();
}