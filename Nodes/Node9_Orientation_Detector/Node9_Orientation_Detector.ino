#include <ThingsBoard.h>
#include <WiFi.h>
#include <ArduinoOTA.h>
#include <ArduinoHttpClient.h>

#include <MPU9250.h>

constexpr char WIFI_SSID[] PROGMEM = "CSD";
constexpr char WIFI_PASSWORD[] PROGMEM = "csd@NITK2014";

constexpr char TOKEN[] PROGMEM = "GiHzHre1VhdFjUggkhFj";

constexpr char THINGSBOARD_SERVER[] PROGMEM = "10.14.0.205";
constexpr uint16_t THINGSBOARD_PORT PROGMEM = 1883U;
constexpr uint32_t MAX_MESSAGE_SIZE PROGMEM = 128U;
constexpr uint32_t SERIAL_DEBUG_BAUD PROGMEM = 115200U;

constexpr char ROLL_KEY[] PROGMEM = "roll";
constexpr char PITCH_KEY[] PROGMEM = "pitch";
constexpr char YAW_KEY[] PROGMEM = "yaw";
constexpr char IMPACT_KEY[] PROGMEM = "impact";
constexpr char CRASH_KEY[] PROGMEM = "crash";
constexpr char FALL_KEY[] PROGMEM = "fall";

constexpr const char RPC_SENSOR_DATA[] PROGMEM = "accelMagnitude";
constexpr const char RPC_CRASH_METHOD[] PROGMEM = "crash";
constexpr const char RPC_FREEFALL_METHOD[] PROGMEM = "freefall";
constexpr const char RPC_SWITCH_KEY[] PROGMEM = "switch";
constexpr const char RPC_RESPONSE_KEY[] PROGMEM = "example_response";

WiFiClient espClient;
ThingsBoard tb(espClient, MAX_MESSAGE_SIZE);

uint8_t status = WL_IDLE_STATUS;  // the Wifi radio's status
bool subscribed = false;
bool requestedShared = false;
int msg = 0;

char *BASE_URL = "/api/v1";   // Define base URL for API requests
char *ENDPOINT = "firmware";  // Define endpoint for firmware updates
char PATH[256];               // Define array to store the path for firmware updates

constexpr const char FW_TITLE_KEY2[] = "fw_title";
constexpr const char FW_VER_KEY2[] = "fw_version";

char CURRENT_VERSION[] = "1.0.0";
constexpr int FIRMWARE_SIZE = 20;           // Adjust the size according to your requirements
char NEW_VERSION[FIRMWARE_SIZE] = "1.0.0";  // Declare NEW_VERSION array

char FW_TITLE[] = "RPi";
constexpr int TITLE_SIZE = 20;       // Adjust the size according to your requirements
char FWW_TITLE[TITLE_SIZE] = "RPi";  // Declare NEW_VERSION array

// Shared attributes we want to request from the server
constexpr std::array<const char *, 2U> REQUESTED_SHARED_ATTRIBUTES = {
  FW_TITLE_KEY2,
  FW_VER_KEY2
};

#define LED_PIN 19     // LED connected to digital pin D18
#define BUZZER_PIN 18  // Buzzer connected to digital pin D5

#define WIFI_STATUS_LED 2

#define BUZZER_VOLUME 0  // Adjust this value to control the buzzer BUZZER_VOLUME

#define DEBOUNCE_TIME 3000
#define BLINK_INTERVAL 250

int CrashState = 0, FreefallState = 0;

MPU9250 mpu;

float accelX_g, accelY_g, accelZ_g, accelMagnitude;
float roll_deg, pitch_deg, yaw_deg;

bool buzzerState = false;

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
void processSharedAttributeRequest(const Shared_Attribute_Data &data) {
  for (auto it = data.begin(); it != data.end(); ++it) {
    Serial.println(it->key().c_str());
    // Shared attributes have to be parsed by their type.
    Serial.println(it->value().as<const char *>());
    if (strcmp_P(it->key().c_str(), FW_VER_KEY2) == 0) {
      // If the key is "CURRENT_VERSION", print its value
      Serial.print("NEW_VERSION: ");
      // Copy the value to NEW_VERSION array
      strncpy(NEW_VERSION, it->value().as<const char *>(), FIRMWARE_SIZE - 1);
      // Ensure null termination
      NEW_VERSION[FIRMWARE_SIZE - 1] = '\0';
      // Print the value
      Serial.println(NEW_VERSION);
    }
    if (strcmp_P(it->key().c_str(), FW_TITLE_KEY2) == 0) {
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
  const unsigned short SERVER_PORT = 8090U;                       // Commonly 80 (HTTP) | 443 (HTTPS)
  HttpClient client(espClient, THINGSBOARD_SERVER, SERVER_PORT);  // HTTP
  // HttpClient client(wifiClientSSL, SERVER, SERVER_PORT);  // HTTPS
  char buff[64];
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

RPC_Response processCrashState(RPC_Data &data) {
  Serial.println("RECIEVED CRASH STATE");
  CrashState = data;
  Serial.println("CRASH STATE CHANGE:");
  Serial.print(CrashState);
  return RPC_Response("actuatorState", CrashState);
}

RPC_Response processFreefallState(RPC_Data &data) {
  Serial.println("RECIEVED FREEFALL STATE");
  FreefallState = data;
  Serial.println("FREEFALL STATE CHANGE:");
  Serial.print(FreefallState);
  return RPC_Response("actuatorState", FreefallState);
}

const std::array<RPC_Callback, 2U> callbacks = {
  RPC_Callback{ RPC_CRASH_METHOD, processCrashState },
  RPC_Callback{ RPC_FREEFALL_METHOD, processFreefallState }
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

  if (mpu.update()) {
    // Read accelerometer data
    accelX_g = mpu.getAccX();
    accelY_g = mpu.getAccY();
    accelZ_g = mpu.getAccZ();
    accelMagnitude = sqrt(accelX_g * accelX_g + accelY_g * accelY_g + accelZ_g * accelZ_g);
  }

  // Adjust the free fall and crash threshold values based on your requirements
  float freeFallThreshold = 0.2;  // You may need to fine-tune this value
  float crashThreshold = 2.0;     // You may need to fine-tune this value
                                  //if (accelMagnitude < freeFallThreshold) {
  if (FreefallState == 40) {
    FreefallState = 0;
    Serial.println("Free fall detected!");
    tb.sendTelemetryString(FALL_KEY, "DETECTED!");
    tb.sendAttributeBool(IMPACT_KEY, true);
    actuate();
  } else {
    tb.sendTelemetryString(FALL_KEY, "NORMAL :)");
    tb.sendAttributeBool(IMPACT_KEY, false);
  }

  if (CrashState == 40) {
    CrashState = 0;
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

  tb.sendTelemetryData(RPC_SENSOR_DATA, accelMagnitude);

  // Uploads new telemetry to ThingsBoard
  // Serial.println(F("Sending data..."));

  tb.loop();
}