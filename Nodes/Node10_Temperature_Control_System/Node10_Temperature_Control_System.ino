#include <ThingsBoard.h>
#include <WiFiNINA.h>
#include <LM35.h>
#include <WiFi.h>

#include <ArduinoOTA.h>
#include <ArduinoHttpClient.h>

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
constexpr uint32_t MAX_MESSAGE_SIZE = 256U;
constexpr uint32_t SERIAL_DEBUG_BAUD = 115200U;

constexpr char TEMPERATURE_KEY[] = "Temperature";
constexpr char ACTUATOR_KEY[] = "actuatorState";

constexpr const char TEMPERATURE_TELEMETRY[] = "temperature_data";
constexpr const char RPC_TEMP_METHOD[] = "temp";

int status = WL_IDLE_STATUS;  // the Wifi radio's status
bool subscribed = false;
int temp_value;

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

char CURRENT_VERSION[] = "1.0.0";
constexpr int FIRMWARE_SIZE = 20;           // Adjust the size according to your requirements
char NEW_VERSION[FIRMWARE_SIZE] = "1.0.0";  // Declare NEW_VERSION array

char FW_TITLE[] = "NANOTC";
constexpr int TITLE_SIZE = 20;          // Adjust the size according to your requirements
char FWW_TITLE[TITLE_SIZE] = "NANOTC";  // Declare NEW_VERSION array

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

LM35 sensor(LM35_PIN);

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

void handleSketchDownload(const char *token, char *title, char *CURRENT_VERSION) {
  sprintf(PATH, "%s/%s/%s?title=%s&version=%s", BASE_URL, token, ENDPOINT, title, CURRENT_VERSION);
  // const char* THINGSBOARD_SERVER = "10.100.80.25";  // Set your correct hostname
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

RPC_Response setTemeratureState(RPC_Data &data) {
  Serial.println("RECIEVED VALUE");
  temp_value = data;
  // Serial.println("VALUE:");
  // Serial.print(temp_value);
  return RPC_Response("actuatorState", 0);
}

const std::array<RPC_Callback, 1U> callbacks = {
  RPC_Callback{ RPC_TEMP_METHOD, setTemeratureState }
};

void setup() {
  // Initalize serial connection for debugging
  Serial.begin(SERIAL_DEBUG_BAUD);
  delay(1000);

  pinMode(LED_PIN, OUTPUT);  // Set LED pin as output
  pinMode(FAN_PIN, OUTPUT);  // Set fan pin as output
  pinMode(WIFI_STATUS_LED, OUTPUT);
  digitalWrite(LED_PIN, LOW);  // Turn off the LED
  digitalWrite(FAN_PIN, LOW);  // Turn off the Fan
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
      handleSketchDownload(TOKEN, FW_TITLE, CURRENT_VERSION);

    } else {
      if (msg == 0) {
        Serial.println("\n");
        Serial.println("updates NOT available...");
        Serial.println("\n");
        msg = 1;
      }
    }
  }

  int temperature = sensor.getTemp();  // Read temperature from LM35 sensor
  Serial.print("Temperature: ");
  Serial.println(temperature);

  tb.sendTelemetryInt(TEMPERATURE_KEY, temperature);

  if (temp_value == 30) {
    temp_value = 0;
    digitalWrite(LED_PIN, HIGH);  // Turn on the LED
    digitalWrite(FAN_PIN, LOW);   // Turn on the Fan
    tb.sendTelemetryString(TEMPERATURE_TELEMETRY, "Temperature Upnormal");
    tb.sendAttributeBool(ACTUATOR_KEY, true);
  } else {
    // Code for the else block
    tb.sendTelemetryString(TEMPERATURE_TELEMETRY, "Temperature Normal");
    tb.sendAttributeBool(ACTUATOR_KEY, false);
    digitalWrite(LED_PIN, LOW);  // Turn off the LED
    digitalWrite(FAN_PIN, LOW);  // Turn off the Fan
  }

  tb.loop();
}