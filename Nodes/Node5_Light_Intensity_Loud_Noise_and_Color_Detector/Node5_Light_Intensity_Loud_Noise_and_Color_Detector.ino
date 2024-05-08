#include <ThingsBoard.h>
#include <WiFiNINA.h>
#include <ArduinoOTA.h>
#include <ArduinoHttpClient.h>

constexpr char WIFI_SSID[] = "CSD";
constexpr char WIFI_PASSWORD[] = "csd@NITK2014";

constexpr char TOKEN[] = "L9dOkkknZXgittW0WAIh";

constexpr char THINGSBOARD_SERVER[] = "10.14.0.205";
constexpr uint16_t THINGSBOARD_PORT = 1883U;
constexpr uint32_t MAX_MESSAGE_SIZE = 128U;
constexpr uint32_t SERIAL_DEBUG_BAUD = 115200U;

constexpr char LOUD_NOISE_KEY[] = "loudNoise";
constexpr char LIGHT_INTENSITY_KEY[] = "lightIntensity";
constexpr char RED_COLOR_KEY[] = "red";
constexpr char GREEN_COLOR_KEY[] = "green";
constexpr char BLUE_COLOR_KEY[] = "blue";
constexpr char ACTUATOR1_KEY[] = "actuator1";
constexpr char ACTUATOR2_KEY[] = "actuator2";
constexpr char ACTUATOR3_KEY[] = "actuator3";

constexpr const char RPC_SOUND_DETECT_KEY[] = "sound";
constexpr const char RPC_LIGHT_DETECT_KEY[] = "light";
constexpr const char RPC_COLOR_KEY[] = "color";
constexpr const char RPC_SWITCH_KEY[] PROGMEM = "switch";
constexpr const char RPC_RESPONSE_KEY[] = "example_response";

WiFiClient espClient;
ThingsBoard tb(espClient, MAX_MESSAGE_SIZE);

uint8_t status = WL_IDLE_STATUS;  // the Wifi radio's status
bool subscribed = false;
bool requestedShared = false;
int msg = 0;

char *BASE_URL = "/api/v1";   // Define base URL for API requests
char *ENDPOINT = "firmware";  // Define endpoint for firmware updates
char PATH[256];               // Define array to store the path for firmware updates

constexpr const char FW_TITLE_KEY[] = "fw_title";
constexpr const char FW_VER_KEY[] = "fw_version";

char CURRENT_VERSION[] = "1.0.0";
constexpr int FIRMWARE_SIZE = 20;           // Adjust the size according to your requirements
char NEW_VERSION[FIRMWARE_SIZE] = "1.0.0";  // Declare NEW_VERSION array

char FW_TITLE[] = "RPi";
constexpr int TITLE_SIZE = 20;       // Adjust the size according to your requirements
char FWW_TITLE[TITLE_SIZE] = "RPi";  // Declare NEW_VERSION array

// Shared attributes we want to request from the server
constexpr std::array<const char *, 2U> REQUESTED_SHARED_ATTRIBUTES = {
  FW_TITLE_KEY,
  FW_VER_KEY
};

#define SOUND_SENSOR_PIN A2
#define LDR_PIN A1

#define S0 5  //Module pins wiring
#define S1 6
#define S2 7
#define S3 8
#define COLOR_lOUT 9
#define COLOR_DETECTOR_LED_PIN 10

#define LED_R 2  //LED pins, don't forget "pwm"
#define LED_G 3
#define LED_B 4

#define SOUND_LED 0
#define LIGHT_LED 1

#define WIFI_STATUS_LED LED_BUILTIN

#define LIGHT_THRESHOLD 25
#define SOUND_THRESHOLD 85

int soundDetected, lightDetected;
int soundStatus, lightStatus;
int Red = 0, Blue = 0, Green = 0;
int rValue = 0, gValue = 0, bValue = 0;

int loudNoiseDetectorEnable = 0, lightIntensityDetectorEnable = 0, colorDetectorEnable = 0;

const int sampleWindow = 50;  // Sample window width in mS (50 mS = 20Hz)
unsigned int sample;

unsigned long previousMillis = 0;  // Stores the last time the LED was updated
const long BLINK_INTERVAL = 500;   // Blinking interval in milliseconds (1 second)

void InitWiFi() {
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to network: ");
    Serial.println(WIFI_SSID);
    status = WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    delay(5000);
  }
  Serial.println("Connected to AP");
}

bool reconnect() {
  // Check to ensure we aren't connected yet
  status = WiFi.status();
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

RPC_Response processSoundDetectEnable(const RPC_Data &data) {
  Serial.println("Received the set temperature RPC method");

  // Process data
  loudNoiseDetectorEnable = data;

  Serial.print("Sound Detect Switch: ");
  Serial.println(loudNoiseDetectorEnable);

  // Just an response example
  StaticJsonDocument<JSON_OBJECT_SIZE(1)> doc;
  doc[RPC_RESPONSE_KEY] = 42;
  return RPC_Response(doc);
}

RPC_Response processLightDetectEnable(const RPC_Data &data) {
  Serial.println("Received the set switch method");

  // Process data
  lightIntensityDetectorEnable = data;

  Serial.print("Light Detect Switch: ");
  Serial.println(lightIntensityDetectorEnable);

  // Just an response example
  StaticJsonDocument<JSON_OBJECT_SIZE(1)> doc;
  doc[RPC_RESPONSE_KEY] = 22.02;
  return RPC_Response(doc);
}

RPC_Response processColorDetectEnable(const RPC_Data &data) {
  Serial.println("Received the set switch method");

  // Process data
  colorDetectorEnable = data;

  Serial.print("Color Detect Switch: ");
  Serial.println(colorDetectorEnable);

  // Just an response example
  StaticJsonDocument<JSON_OBJECT_SIZE(1)> doc;
  doc[RPC_RESPONSE_KEY] = 22.02;
  return RPC_Response(doc);
}

const std::array<RPC_Callback, 3U> callbacks = {
  RPC_Callback{ RPC_SOUND_DETECT_KEY, processSoundDetectEnable },
  RPC_Callback{ RPC_LIGHT_DETECT_KEY, processLightDetectEnable },
  RPC_Callback{ RPC_COLOR_KEY, processColorDetectEnable }
};

void getColor() {
  digitalWrite(S2, LOW);  //S2/S3 levels define which set of photodiodes we are using LOW/LOW is for RED LOW/HIGH is for Blue and HIGH/HIGH is for green
  digitalWrite(S3, LOW);
  Red = pulseIn(COLOR_OUT, digitalRead(COLOR_OUT) == HIGH ? LOW : HIGH);  //here we wait until "out" go LOW, we start measuring the duration and stops when "out" is HIGH again, if you have trouble with this expression check the bottom of the code
  delay(20);
  digitalWrite(S3, HIGH);  //Here we select the other color (set of photodiodes) and measure the other colors value using the same techinque
  Blue = pulseIn(COLOR_OUT, digitalRead(COLOR_OUT) == HIGH ? LOW : HIGH);
  delay(20);
  digitalWrite(S2, HIGH);
  Green = pulseIn(COLOR_OUT, digitalRead(COLOR_OUT) == HIGH ? LOW : HIGH);
  delay(20);
}

void blinkLED(int duration) {

  unsigned long startMillis = millis();
  unsigned long previousMillis = millis();

  unsigned long interval = BLINK_INTERVAL;

  while (previousMillis - startMillis < duration) {
    if (millis() - previousMillis >= interval) {
      previousMillis = millis();
      digitalWrite(SOUND_LED, !digitalRead(SOUND_LED));
    }
  }
  digitalWrite(SOUND_LED, LOW);
}

void setup() {
  // Initalize serial connection for debugging
  Serial.begin(SERIAL_DEBUG_BAUD);
  delay(1000);

  pinMode(SOUND_LED, OUTPUT);
  pinMode(LIGHT_LED, OUTPUT);

  pinMode(LED_R, OUTPUT);
  pinMode(LED_G, OUTPUT);
  pinMode(LED_B, OUTPUT);

  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(COLOR_OUT, INPUT);
  pinMode(COLOR_DETECTOR_LED_PIN, OUTPUT);

  pinMode(WIFI_STATUS_LED, OUTPUT);

  digitalWrite(S0, HIGH);  //Putting S0/S1 on HIGH/HIGH levels means the output frequency scalling is at 100% (recommended)
  digitalWrite(S1, HIGH);  //LOW/LOW is off HIGH/LOW is 20% and LOW/HIGH is  2%

  digitalWrite(COLOR_DETECTOR_LED_PIN, LOW);

  digitalWrite(WIFI_STATUS_LED, LOW);

  InitWiFi();
}

void loop() {

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

  //Loud Sound Detector
  if (loudNoiseDetectorEnable) {
    unsigned long startMillis = millis();  // Start of sample window
    float peakToPeak = 0;                  // peak-to-peak level

    unsigned int signalMax = 0;    //minimum value
    unsigned int signalMin = 280;  //maximum value

    while (millis() - startMillis < sampleWindow) {
      sample = analogRead(SOUND_SENSOR_PIN);  //get reading from microphone
      if (sample < 1024)                      // toss out spurious readings
      {
        if (sample > signalMax) {
          signalMax = sample;  // save just the max levels
        } else if (sample < signalMin) {
          signalMin = sample;  // save just the min levels
        }
      }
    }

    peakToPeak = signalMax - signalMin;  // max - min = peak-peak amplitude
    float sounddB = map(peakToPeak, 10, 280, 55, 100);
    // Serial.println(sounddB);

    if (sounddB >= SOUND_THRESHOLD) {
      Serial.println("Loud Sound Detected");
      blinkLED(2000);
      soundStatus = 1;
      tb.sendTelemetryString(LOUD_NOISE_KEY, "Loud Noise Detected!");
    } else {
      digitalWrite(SOUND_LED, LOW);
      soundStatus = 0;
      tb.sendTelemetryString(LOUD_NOISE_KEY, "Normal");
    }
  } else {
    digitalWrite(SOUND_LED, LOW);
  }

  //Light Intensity Detector
  if (lightIntensityDetectorEnable) {
    lightDetected = analogRead(LDR_PIN);

    if (lightDetected <= LIGHT_THRESHOLD) {
      Serial.println("Light not Detected");
      digitalWrite(LIGHT_LED, HIGH);
      soundStatus = 1;
      tb.sendTelemetryString(LIGHT_INTENSITY_KEY, "Light Intensity Reduced!");
    } else {
      digitalWrite(LIGHT_LED, LOW);
      soundStatus = 0;
      tb.sendTelemetryString(LIGHT_INTENSITY_KEY, "Normal");
    }
  } else {
    digitalWrite(LIGHT_LED, LOW);
  }

  //Color Detector
  if (colorDetectorEnable) {
    digitalWrite(COLOR_DETECTOR_LED_PIN, HIGH);
    getColor();  //Execute the getColor function

    rValue = map(Red, 15, 60, 255, 0);
    gValue = map(Green, 30, 55, 255, 0);
    bValue = map(Blue, 13, 45, 255, 0);

    analogWrite(LED_R, rValue);
    analogWrite(LED_G, gValue);
    analogWrite(LED_B, bValue);

    tb.sendTelemetryInt(RED_COLOR_KEY, rValue);
    tb.sendTelemetryInt(GREEN_COLOR_KEY, gValue);
    tb.sendTelemetryInt(BLUE_COLOR_KEY, bValue);
  } else {
    digitalWrite(COLOR_DETECTOR_LED_PIN, LOW);
    analogWrite(LED_R, 0);
    analogWrite(LED_G, 0);
    analogWrite(LED_B, 0);
  }

  // Uploads new telemetry to ThingsBoard using MQTT.
  // Serial.println("Sending data...");

  tb.loop();
}
