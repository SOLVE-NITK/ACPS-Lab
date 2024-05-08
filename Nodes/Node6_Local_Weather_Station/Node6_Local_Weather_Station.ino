#include <ThingsBoard.h>
#include <WiFiNINA.h>
#include <ArduinoOTA.h>
#include <ArduinoHttpClient.h>

#include <Wire.h>
#include <NTPClient.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_BMP280.h>
#include <DHT.h>
#include <DS3231.h>

constexpr char WIFI_SSID[] = "CSD";
constexpr char WIFI_PASSWORD[] = "csd@NITK2014";

constexpr char TOKEN[] = "s3ryZWlLvKnucI32qqT7";

constexpr char THINGSBOARD_SERVER[] = "10.14.0.205";
constexpr uint16_t THINGSBOARD_PORT = 1883U;
constexpr uint32_t MAX_MESSAGE_SIZE = 128U;
constexpr uint32_t SERIAL_DEBUG_BAUD = 115200U;

constexpr char TEMPERATURE_KEY[] = "temperature";
constexpr char HUMIDITY_KEY[] = "humidity";
constexpr char HEAT_INDEX_KEY[] = "heatIndex";
constexpr char PRESSURE_KEY[] = "atmosphericPressure";
constexpr char ALTITUDE_KEY[] = "altitude";
constexpr char YEAR_KEY[] = "year";
constexpr char MONTH_KEY[] = "month";
constexpr char DATE_KEY[] = "date";
constexpr char DAY_KEY[] = "day";
constexpr char HOUR_KEY[] = "hour";
constexpr char MINUTE_KEY[] = "minute";
constexpr char SECOND_KEY[] = "second";

constexpr const char RPC_TEMPERATURE_METHOD[] = "example_set_temperature";
constexpr const char RPC_SWITCH_METHOD[] = "example_set_switch";
constexpr const char RPC_TEMPERATURE_KEY[] = "temp";
constexpr const char RPC_SWITCH_KEY[] PROGMEM = "switch";
constexpr const char RPC_RESPONSE_KEY[] = "example_response";

WiFiClient espClient;
WiFiUDP udp;
ThingsBoard tb(espClient, MAX_MESSAGE_SIZE);
NTPClient timeClient(udp, THINGSBOARD_SERVER, gmtOffsetInSeconds);

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

const long gmtOffsetInSeconds = 19800;  // Your GMT offset in seconds

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C  // Address of the OLED display

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define DHTPIN 2
#define DHTTYPE DHT21  // DHT 21 (AM2301)

#define WIFI_STATUS_LED LED_BUILTIN

Adafruit_BMP280 bmp;  // I2C
DHT dht(DHTPIN, DHTTYPE);
RTClib myRTC;
DS3231 myRTC2;

float temp, humid, hI, pressure, alti;
int year, month, date, day, hour, minute, second;
const char* daysOfWeek[] = { "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday" };
const char* monthsOfYear[] = { "January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December" };

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

RPC_Response processTemperatureChange(const RPC_Data& data) {
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

RPC_Response processSwitchChange(const RPC_Data& data) {
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

void updateRTCFromNTP() {
  timeClient.begin();
  timeClient.update();

  unsigned long epochTime = timeClient.getEpochTime();
  myRTC2.setEpoch(epochTime);

  Serial.print("Updated RTC with NTP time: ");
  Serial.println(epochTime);
}

void setup() {
  // Initalize serial connection for debugging
  Serial.begin(SERIAL_DEBUG_BAUD);

  pinMode(WIFI_STATUS_LED, OUTPUT);

  digitalWrite(WIFI_STATUS_LED, LOW);

  InitWiFi();
  Wire.begin();

  delay(1000);

  bmp.begin(BMP280_ADDRESS_ALT, BMP280_CHIPID);
  dht.begin();

  myRTC2.setClockMode(false);  // set to 24h
  // Update RTC with NTP time
  updateRTCFromNTP();

  /* Default settings from datasheet. */
  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                  Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                  Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */

  display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
  display.setRotation(2);  // Rotate 180 degrees
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
    digitalWrite(WIFI_STATUS_LED, LOW);

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

  DateTime now = myRTC.now();

  temp = (dht.readTemperature() + bmp.readTemperature() + myRTC2.getTemperature()) / 3;
  humid = dht.readHumidity();
  hI = dht.computeHeatIndex(temp, humid, false);
  pressure = bmp.readPressure() / 100.00;
  alti = bmp.readAltitude(1013.25);
  year = now.year();
  month = now.month();
  date = now.day();
  day = myRTC2.getDoW();
  hour = now.hour();
  minute = now.minute();
  second = now.second();

  // Uploads new telemetry to ThingsBoard using MQTT
  Serial.println("Sending data...");
  tb.sendTelemetryFloat(TEMPERATURE_KEY, temp);
  tb.sendTelemetryFloat(HUMIDITY_KEY, humid);
  tb.sendTelemetryFloat(HEAT_INDEX_KEY, hI);
  tb.sendTelemetryFloat(PRESSURE_KEY, pressure);
  tb.sendTelemetryFloat(ALTITUDE_KEY, alti);
  tb.sendTelemetryInt(YEAR_KEY, year);
  tb.sendTelemetryInt(MONTH_KEY, month);
  tb.sendTelemetryInt(DATE_KEY, date);
  tb.sendTelemetryInt(DAY_KEY, day);
  tb.sendTelemetryInt(HOUR_KEY, hour);
  tb.sendTelemetryInt(MINUTE_KEY, minute);
  tb.sendTelemetryInt(SECOND_KEY, second);

  Serial.print(F("Humidity: "));
  Serial.print(humid);
  Serial.println(F("%"));

  Serial.print(F("Temperature: "));
  Serial.print(temp);
  Serial.println(F("°C "));

  Serial.print(F("Heat index: "));
  Serial.print(hI);
  Serial.println(F("°C "));

  Serial.print(F("Pressure: "));
  Serial.print(pressure);
  Serial.println(" hPa");

  Serial.print(F("Approx altitude: "));
  Serial.print(alti);
  Serial.println(" m");

  Serial.print(F("Date: "));
  Serial.print(monthsOfYear[month]);
  Serial.print(" ");
  Serial.print(date);
  Serial.print(", ");
  Serial.println(year);
  Serial.print(F("Day: "));
  Serial.println(daysOfWeek[day]);
  Serial.print(F("Time: "));
  if (hour < 10) {
    Serial.print("0");
  }
  Serial.print(hour);
  Serial.print(":");
  if (minute < 10) {
    Serial.print("0");
  }
  Serial.print(minute);
  Serial.print(":");
  if (second < 10) {
    Serial.print("0");
  }
  Serial.println(second);
  Serial.println();

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  display.setCursor(0, 0);
  display.print(F("Humidity: "));
  display.print(humid, 1);
  display.println(F("%"));

  display.print(F("Temperature: "));
  display.print(temp, 1);
  display.print((char)247);
  display.println(F("C"));

  display.print(F("Heat index: "));
  display.print(hI, 1);
  display.print((char)247);
  display.println(F("C"));

  display.print(F("Pressure: "));
  display.print(pressure, 1);
  display.println(" hPa");

  display.print(F("Altitude: "));
  display.print(alti, 1);
  display.println(" m");

  display.print(F("Date: "));
  display.print(monthsOfYear[month - 1]);
  display.print(" ");
  display.print(date);
  display.print(", ");
  display.println(year);
  display.print(F("Day: "));
  display.println(daysOfWeek[day - 1]);
  display.print(F("Time: "));
  if (hour < 10) {
    display.print("0");
  }
  display.print(hour);
  display.print(":");
  if (minute < 10) {
    display.print("0");
  }
  display.print(minute);
  display.print(":");
  if (second < 10) {
    display.print("0");
  }
  display.println(second);

  display.display();

  tb.loop();
}