#include <ThingsBoard.h>
#include <WiFiNINA.h>
#include <Wire.h>
#include <NTPClient.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_BMP280.h>
#include <DHT.h>
#include <DS3231.h>

// constexpr char WIFI_SSID[] = "NITK-NET";
// constexpr char WIFI_PASSWORD[] = "2K16NITK";

constexpr char WIFI_SSID[] = "CSD";
constexpr char WIFI_PASSWORD[] = "csd@NITK2014";

constexpr char TOKEN[] = "MexxMcLCT4RCjk9IdKO3";

constexpr char THINGSBOARD_SERVER[] = "10.100.80.26";
constexpr uint16_t THINGSBOARD_PORT = 1883U;
constexpr uint32_t MAX_MESSAGE_SIZE = 128U;
constexpr uint32_t SERIAL_DEBUG_BAUD = 115200U;

IPAddress localIP(10, 100, 80, 39);
IPAddress gateway(10, 100, 80, 1);
IPAddress subnet(255, 255, 252, 0);
IPAddress dns(0, 0, 0, 0);

IPAddress dns1(10, 20, 1, 22);
IPAddress dns2(10, 3, 0, 101);

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

const long gmtOffsetInSeconds = 19800;  // Your GMT offset in seconds

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C  // Address of the OLED display

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define DHTPIN 2
#define DHTTYPE DHT21  // DHT 21 (AM2301)

#define WIFI_STATUS_LED LED_BUILTIN

WiFiClient espClient;
WiFiUDP udp;
ThingsBoard tb(espClient, MAX_MESSAGE_SIZE);
NTPClient timeClient(udp, THINGSBOARD_SERVER, gmtOffsetInSeconds);
Adafruit_BMP280 bmp;  // I2C
DHT dht(DHTPIN, DHTTYPE);
RTClib myRTC;
DS3231 myRTC2;

int status = WL_IDLE_STATUS;  // the Wifi radio's status
bool subscribed = false;

float temp, humid, hI, pressure, alti;
int year, month, date, day, hour, minute, second;
const char* daysOfWeek[] = { "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday" };
const char* monthsOfYear[] = { "January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December" };

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
    delay(5000);
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

/// Processes function for RPC call "example_set_switch"
/// RPC_Data is a JSON variant, that can be queried using operator[]
/// See https://arduinojson.org/v5/api/jsonvariant/subscript/ for more details
/// "data" Data containing the rpc data that was called and its current value
/// Response that should be sent to the cloud. Useful for getMethods
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