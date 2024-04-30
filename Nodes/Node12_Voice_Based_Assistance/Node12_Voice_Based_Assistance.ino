#include <ThingsBoard.h>
#include <WiFiEspClient.h>
#include <WiFiEsp.h>
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <Arduino.h>
#include <WiFiEsp.h>
#include <WiFiEspUdp.h>

// constexpr char WIFI_SSID[] = "NITK-NET";
// constexpr char WIFI_PASSWORD[] = "2K16NITK";

constexpr char WIFI_SSID[] = "CSD";
constexpr char WIFI_PASSWORD[] = "csd@NITK2014";

constexpr char TOKEN[] = "XbhsogGbhVLgYrHSJtB1";

constexpr char THINGSBOARD_SERVER[] = "10.100.80.26";
constexpr uint16_t THINGSBOARD_PORT = 1883U;
constexpr uint32_t MAX_MESSAGE_SIZE = 128U;
constexpr uint32_t SERIAL_DEBUG_BAUD = 115200U;
constexpr uint32_t SERIAL_ESP8266_DEBUG_BAUD = 115200U;

IPAddress localIP(10, 100, 80, 46);
IPAddress gateway(10, 100, 80, 1);
IPAddress subnet(255, 255, 252, 0);
IPAddress dns(0, 0, 0, 0);

IPAddress dns1(10, 20, 1, 22);
IPAddress dns2(10, 3, 0, 101);

IPAddress serverIP(10, 100, 80, 26);  // Server IP address
unsigned int serverPort = 9876;       // Server port
unsigned int localPort = 54321;       // Local port for UDP client

constexpr char LIGHT_KEY[] = "light";
constexpr char FAN_KEY[] = "fan";
constexpr char VOICE_KEY[] = "voice";

constexpr const char RPC_TEMPERATURE_METHOD[] = "example_set_temperature";
constexpr const char RPC_SWITCH_METHOD[] = "example_set_switch";
constexpr const char RPC_TEMPERATURE_KEY[] = "temp";
constexpr const char RPC_SWITCH_KEY[] PROGMEM = "switch";
constexpr const char RPC_RESPONSE_KEY[] = "example_response";

// Serial driver for ESP
#define ESP8266SERIAL Serial1
// Initialize the Ethernet client object
WiFiEspClient espClient;
WiFiEspUDP udpClient;
// Initialize ThingsBoard instance
ThingsBoard tb(espClient, MAX_MESSAGE_SIZE);

int status = WL_IDLE_STATUS;  // the Wifi radio's status
bool subscribed = false;

AudioInputAnalog adc1;    //xy=187,195
AudioRecordQueue queue1;  //xy=385,195
AudioConnection patchCord1(adc1, queue1);

#define SDCARD_CS_PIN BUILTIN_SDCARD

#define PUSH_BUTTON_PIN 2
#define LED_PIN 3
#define FAN_PIN 4

#define WIFI_STATUS_LED LED_BUILTIN

// Flag to indicate to start recording
volatile bool startRecordingFlag = false;

// Remember which mode we're doing
int mode = 0;  // 0=stopped, 1=recording

// The file where data is recorded
File wavFile;
const char *filename = "RECORD.wav";  // Change the filename to whatever you prefer

unsigned long previousMillis = 0;
const long interval = 2000;

char responseBuffer[255];  // Adjust the buffer size as needed

/// @brief Initalizes WiFi connection,
// will endlessly delay until a connection has been successfully established
void InitWiFi() {

  // Configure static IP and DNS server
  WiFi.config(localIP);
  // WiFi.config(localIP, dns, gateway, subnet);
  // WiFi.setDNS(dns1, dns2);

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

void buttonInterrupt() {
  startRecordingFlag = true;
}

void startRecording() {
  Serial.println("startRecording");
  if (SD.exists(filename)) {
    SD.remove(filename);
  }
  wavFile = SD.open(filename, FILE_WRITE);
  if (wavFile) {
    queue1.begin();
    mode = 1;
  }
}

void continueRecording() {
  if (queue1.available() >= 2) {
    byte buffer[512];
    memcpy(buffer, queue1.readBuffer(), 256);
    queue1.freeBuffer();
    memcpy(buffer + 256, queue1.readBuffer(), 256);
    queue1.freeBuffer();
    wavFile.write(buffer, 512);
  }
}

void stopRecording() {
  Serial.println("stopRecording");
  queue1.end();
  if (mode == 1) {
    while (queue1.available() > 0) {
      wavFile.write((byte *)queue1.readBuffer(), 256);
      queue1.freeBuffer();
    }
    updateWavHeader();
    wavFile.close();
  }
  mode = 0;
}

void updateWavHeader() {
  wavFile.seek(0);  // Go to the beginning of the file

  // Write RIFF chunk descriptor
  wavFile.write("RIFF");
  uint32_t fileSize = wavFile.size() - 8;
  wavFile.write((byte *)&fileSize, 4);
  wavFile.write("WAVE");

  // Write fmt subchunk
  wavFile.write("fmt ");
  uint32_t fmtSize = 16;
  wavFile.write((byte *)&fmtSize, 4);
  uint16_t audioFormat = 1;  // PCM format
  wavFile.write((byte *)&audioFormat, 2);
  uint16_t numChannels = 1;  // Mono
  wavFile.write((byte *)&numChannels, 2);
  uint32_t sampleRate = 44100;  // Change to your sample rate
  wavFile.write((byte *)&sampleRate, 4);
  uint32_t byteRate = sampleRate * numChannels * 2;  // 16-bit audio
  wavFile.write((byte *)&byteRate, 4);
  uint16_t blockAlign = numChannels * 2;
  wavFile.write((byte *)&blockAlign, 2);
  uint16_t bitsPerSample = 16;  // Change to your bit depth
  wavFile.write((byte *)&bitsPerSample, 2);

  // Write data subchunk
  wavFile.write("data");
  uint32_t dataLength = wavFile.size() - 44;
  wavFile.write((byte *)&dataLength, 4);
}

void setup() {
  // initialize serial for debugging
  Serial.begin(SERIAL_DEBUG_BAUD);
  // initialize serial for ESP module
  ESP8266SERIAL.begin(SERIAL_ESP8266_DEBUG_BAUD);
  delay(1000);

  pinMode(PUSH_BUTTON_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(PUSH_BUTTON_PIN), buttonInterrupt, FALLING);

  pinMode(LED_PIN, OUTPUT);
  pinMode(FAN_PIN, OUTPUT);
  pinMode(WIFI_STATUS_LED, OUTPUT);

  digitalWrite(LED_PIN, LOW);
  digitalWrite(FAN_PIN, LOW);
  digitalWrite(WIFI_STATUS_LED, LOW);

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

  // Initialize UDP client
  udpClient.begin(localPort);
  delay(2000);

  AudioMemory(60);

  if (!SD.begin(SDCARD_CS_PIN)) {
    while (1) {
      Serial.println("Unable to access the SD card");
      delay(500);
    }
  }
}

void loop() {
  delay(1000);

  if (!reconnect()) {
    digitalWrite(WIFI_STATUS_LED, LOW);
    return;
  }

  if (!tb.connected()) {
    // Reconnect to the ThingsBoard server,
    // if a connection was disrupted or has not yet been established
    char message[ThingsBoard::detectSize("Connecting to: (%s) with token (%s)", THINGSBOARD_SERVER, TOKEN)];
    snprintf_P(message, sizeof(message), "Connecting to: (%s) with token (%s)", THINGSBOARD_SERVER, TOKEN);
    Serial.println(message);
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

  if (startRecordingFlag) {
    startRecording();
    unsigned long currentMillis = millis();
    previousMillis = millis();
    while (previousMillis - currentMillis <= interval) {
      previousMillis = millis();
      continueRecording();
    }
    stopRecording();

    // Now that recording is complete and saved, read and send audio data over UDP
    if (SD.exists(filename)) {
      File wavFile = SD.open(filename, FILE_READ);
      while (wavFile.available()) {
        byte buffer[1470];
        int bytesRead = wavFile.read(buffer, 1470);
        if (bytesRead > 0) {
          udpClient.beginPacket(serverIP, serverPort);
          udpClient.write(buffer, bytesRead);
          udpClient.endPacket();
          Serial.print("Sent ");
          Serial.print(bytesRead);
          Serial.println(" bytes over UDP");
          delay(5);  // Add a small delay to prevent overwhelming the UDP connection
        }
      }
      wavFile.close();
      Serial.println("Audio data sending complete");

      // Send stop signal to the server
      udpClient.beginPacket(serverIP, serverPort);
      udpClient.write("stop_sending");
      udpClient.endPacket();
      Serial.println("Sent stop signal to server");
    }
    delay(2000);
    // Receive and print the response from the server
    int packetSize = udpClient.parsePacket();
    if (packetSize > 0) {
      int bytesRead = udpClient.read(responseBuffer, sizeof(responseBuffer) - 1);
      if (bytesRead > 0) {
        responseBuffer[bytesRead] = '\0';  // Null-terminate the received data
        Serial.println("Received response from server: " + String(responseBuffer));

        tb.sendTelemetryString(VOICE_KEY, String(responseBuffer).c_str());

        if (String(responseBuffer) == "turn on light") {
          Serial.println("Turning on light");
          tb.sendAttributeBool(LIGHT_KEY, true);
          digitalWrite(LED_PIN, HIGH);
        } else if (String(responseBuffer) == "turn off light") {
          Serial.println("Turning off light");
          digitalWrite(LED_PIN, LOW);
          tb.sendAttributeBool(LIGHT_KEY, false);
        } else if (String(responseBuffer) == "turn on fan") {
          Serial.println("Turning on fan");
          digitalWrite(FAN_PIN, HIGH);
          tb.sendAttributeBool(FAN_KEY, true);
        } else if (String(responseBuffer) == "turn off fan") {
          Serial.println("Turning off fan");
          digitalWrite(FAN_PIN, LOW);
          tb.sendAttributeBool(FAN_KEY, false);
        } else {
          Serial.println("Invalid Request");
        }
      }
    }
    startRecordingFlag = false;
  }

  Serial.println("Sending data...");

  tb.loop();
}
