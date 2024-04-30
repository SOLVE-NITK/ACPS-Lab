#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <Arduino.h>
#include <WiFiEsp.h>
#include <WiFiEspUdp.h>

constexpr char WIFI_SSID[] = "NITK-NET";
constexpr char WIFI_PASSWORD[] = "2K16NITK";
IPAddress serverIP(10, 100, 80, 26);  // Server IP address
unsigned int serverPort = 9876;        // Server port
unsigned int localPort = 54321;        // Local port for UDP client

WiFiEspUDP udpClient;

constexpr uint32_t SERIAL_ESP8266_DEBUG_BAUD = 115200U;

#define ESP8266SERIAL Serial1

// GUItool: begin automatically generated code
AudioInputAnalog adc1;    //xy=187,195
AudioRecordQueue queue1;  //xy=385,195
AudioConnection patchCord1(adc1, queue1);
// GUItool: end automatically generated code

// Use these with the Teensy 3.5 & 3.6 & 4.1 SD card
#define SDCARD_CS_PIN BUILTIN_SDCARD

// Remember which mode we're doing
int mode = 0;  // 0=stopped, 1=recording

// The file where data is recorded
File wavFile;
const char *filename = "RECORD.wav";  // Change the filename to whatever you prefer

unsigned long previousMillis = 0;
const long interval = 2000;

void InitWiFi() {
  Serial.println("Connecting to AP ...");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to AP");
}

bool reconnect() {
  const uint8_t status = WiFi.status();
  if (status == WL_CONNECTED) {
    return true;
  }

  InitWiFi();
  return true;
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
  Serial.begin(115200);
  ESP8266SERIAL.begin(SERIAL_ESP8266_DEBUG_BAUD);
  delay(1000);

  WiFi.init(&ESP8266SERIAL);
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
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

  if (!reconnect()) {
    return;
  }

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
  }

  delay(5000);
}