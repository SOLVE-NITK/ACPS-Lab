#include <ThingsBoard.h>
#include <WiFiEspClient.h>
#include <WiFiEsp.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_MLX90614.h>

// constexpr char WIFI_SSID[] = "NITK-NET";
// constexpr char WIFI_PASSWORD[] = "2K16NITK";

constexpr char WIFI_SSID[] = "CSD";
constexpr char WIFI_PASSWORD[] = "csd@NITK2014";

constexpr char TOKEN[] = "lAYoHsFrPwAgzbiyie6o";

constexpr char THINGSBOARD_SERVER[] = "10.100.80.26";
constexpr uint16_t THINGSBOARD_PORT = 1883U;
constexpr uint32_t MAX_MESSAGE_SIZE = 128U;
constexpr uint32_t SERIAL_DEBUG_BAUD = 115200U;
constexpr uint32_t SERIAL_ESP8266_DEBUG_BAUD = 115200U;

IPAddress localIP(10, 100, 80, 36);
IPAddress gateway(10, 100, 80, 1);
IPAddress subnet(255, 255, 252, 0);
IPAddress dns(0, 0, 0, 0);

IPAddress dns1(10, 20, 1, 22);
IPAddress dns2(10, 3, 0, 101);

constexpr char AMBIENT_TEMP_C_KEY[] = "ambientC";
constexpr char OBJECT_TEMP_C_KEY[] = "objectC";
constexpr char AMBIENT_TEMP_F_KEY[] = "ambientF";
constexpr char OBJECT_TEMP_F_KEY[] = "objectF";
constexpr char ABNORMAL_TEMP_KEY[] = "abnormalTemp";
constexpr char THRESHOLD_TEMP_KEY[] = "thresholdTemp";

constexpr const char RPC_TEMPERATURE_METHOD[] = "example_set_temperature";
constexpr const char RPC_SWITCH_METHOD[] = "example_set_switch";
constexpr const char RPC_TEMPERATURE_KEY[] = "temp";
constexpr const char RPC_SWITCH_KEY[] PROGMEM = "switch";
constexpr const char RPC_RESPONSE_KEY[] = "example_response";

// Serial driver for ESP
#define ESP8266SERIAL Serial1
// Initialize the Ethernet client object
WiFiEspClient espClient;
// Initialize ThingsBoard instance
ThingsBoard tb(espClient, MAX_MESSAGE_SIZE);

int status = WL_IDLE_STATUS;  // the Wifi radio's status
bool subscribed = false;

#define SSD1306_I2C_ADDRESS 0x3C
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define OLED_RESET -1

// Pin definitions for the push buttons and LEDs
#define BUTTON_FAHRENHEIT_PIN 3
#define BUTTON_CELSIUS_PIN 4
#define LED_PIN_HIGH_TEMP 2

#define WIFI_STATUS_LED LED_BUILTIN

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
Adafruit_MLX90614 mlx;

volatile boolean displayInCelsius = true;
const float thresholdTempC = 30.0;  // Adjust the threshold temperature in Celsius

/// @brief Initalizes WiFi connection,
// will endlessly delay until a connection has been successfully established
void InitWiFi() {

  // Configure static IP and DNS server
  WiFi.config(localIP);
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

void celsiusButtonISR() {
  displayInCelsius = true;
}

void fahrenheitButtonISR() {
  displayInCelsius = false;
}

void setup() {
  // initialize serial for debugging
  Serial.begin(SERIAL_DEBUG_BAUD);
  // initialize serial for ESP module
  ESP8266SERIAL.begin(SERIAL_ESP8266_DEBUG_BAUD);

  Wire.begin();
  mlx.begin();

  delay(1000);

  // Set the push button pins as inputs with internal pull-ups
  pinMode(BUTTON_CELSIUS_PIN, INPUT_PULLUP);
  pinMode(BUTTON_FAHRENHEIT_PIN, INPUT_PULLUP);

  // Set the LED pins as outputs
  pinMode(LED_PIN_HIGH_TEMP, OUTPUT);

  pinMode(WIFI_STATUS_LED, OUTPUT);

  digitalWrite(LED_PIN_HIGH_TEMP, LOW);  // Initialize the LED as off
  digitalWrite(WIFI_STATUS_LED, LOW);

  // Attach interrupts to the push buttons
  attachInterrupt(digitalPinToInterrupt(BUTTON_CELSIUS_PIN), celsiusButtonISR, FALLING);
  attachInterrupt(digitalPinToInterrupt(BUTTON_FAHRENHEIT_PIN), fahrenheitButtonISR, FALLING);

  if (!display.begin(SSD1306_SWITCHCAPVCC, SSD1306_I2C_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ;
  }

  display.display();
  delay(2000);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);


  // initialize ESP module
  WiFi.init(&ESP8266SERIAL);
  // check for the presence of the shield
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue
    while (true)
      ;
  }
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
    char message[ThingsBoard::detectSize("Connecting to: (%s) with token (%s)", THINGSBOARD_SERVER, TOKEN)];
    snprintf_P(message, sizeof(message), "Connecting to: (%s) with token (%s)", THINGSBOARD_SERVER, TOKEN);
    Serial.println(message);
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

  // Read the temperatures
  float objectTempC = mlx.readObjectTempC();
  float objectTempF = mlx.readObjectTempF();
  float ambientTempC = mlx.readAmbientTempC();
  float ambientTempF = mlx.readAmbientTempF();

  // Uploads new telemetry to ThingsBoard using MQTT.
  tb.sendTelemetryFloat(AMBIENT_TEMP_C_KEY, ambientTempC);
  tb.sendTelemetryFloat(OBJECT_TEMP_C_KEY, objectTempC);
  tb.sendTelemetryFloat(AMBIENT_TEMP_F_KEY, ambientTempF);
  tb.sendTelemetryFloat(OBJECT_TEMP_F_KEY, objectTempF);

  // Update the display based on the value of displayInCelsius
  display.clearDisplay();
  display.setCursor(0, 0);

  display.setTextColor(SSD1306_WHITE);
  if (displayInCelsius) {
    display.setTextSize(1, 2);  // Smaller text size for the first line
    display.print("Room: ");
    display.setTextSize(2);  // Bigger text size for the first line
    display.print(ambientTempC, 2);
    display.print(" C");

  } else {
    display.setTextSize(1, 2);  // Smaller text size for the first line
    display.print("Room: ");
    display.setTextSize(2);  // Bigger text size for the first line
    display.print(ambientTempF, 2);
    display.print(" F");
  }

  display.setTextSize(2);  // Bigger text size for the second line
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 17);  // Move the cursor to the second line
  if (displayInCelsius) {
    display.setTextSize(1, 2);  // Smaller text size for the first line
    display.print("Body: ");
    display.setTextSize(2);  // Bigger text size for the first line
    display.print(objectTempC, 2);
    display.print(" C");
  } else {
    display.setTextSize(1, 2);  // Smaller text size for the first line
    display.print("Body: ");
    display.setTextSize(2);  // Bigger text size for the first line
    display.print(objectTempF, 2);
    display.print(" F");
  }

  // Check if the object temperature crosses the high threshold
  if (objectTempC > thresholdTempC) {
    tb.sendTelemetryString(ABNORMAL_TEMP_KEY, "ABNORMAL!");
    tb.sendAttributeBool(THRESHOLD_TEMP_KEY, true);
    digitalWrite(LED_PIN_HIGH_TEMP, HIGH);  // Turn on the high-temperature LED
  } else {
    tb.sendTelemetryString(ABNORMAL_TEMP_KEY, "NORMAL :)");
    tb.sendAttributeBool(THRESHOLD_TEMP_KEY, false);
    digitalWrite(LED_PIN_HIGH_TEMP, LOW);  // Turn off the high-temperature LED
  }

  display.display();

  tb.loop();
}
