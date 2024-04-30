#include <Arduino.h>
#include <WiFiEsp.h>
#include <WiFiEspClient.h>
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>

constexpr char WIFI_SSID[] = "NITK-NET";
constexpr char WIFI_PASSWORD[] = "2K16NITK";

// constexpr char WIFI_SSID[] = "CSD";
// constexpr char WIFI_PASSWORD[] = "csd@NITK2014";
char server[] = "10.100.80.26";
const int serverPort = 9876;

constexpr uint32_t SERIAL_ESP8266_DEBUG_BAUD = 115200U;

#define ESP8266SERIAL Serial1

WiFiEspClient client;

AudioInputAnalog adc1;    //xy=187,195
AudioRecordQueue queue1;  //xy=385,195
AudioConnection patchCord1(adc1, queue1);

// Use these with the Teensy 3.5 & 3.6 & 4.1 SD card
#define SDCARD_CS_PIN BUILTIN_SDCARD

// Remember which mode we're doing
int mode = 0;  // 0=stopped, 1=recording

// The file where data is recorded
File wavFile;
const char* filename = "RECORD.wav";  // Change the filename to whatever you prefer

unsigned long previousMillis = 0;
const long interval = 5000;

// Define the chunk size for sending data
const int CHUNK_SIZE = 4096;

uint32_t fmtSize = 16;
uint16_t audioFormat = 1;                          // PCM format
uint16_t numChannels = 1;                          // Mono
uint32_t sampleRate = 44100;                       // Change to your sample rate
uint32_t byteRate = sampleRate * numChannels * 2;  // 16-bit audio
uint16_t blockAlign = numChannels * 2;
uint16_t bitsPerSample = 16;  // Change to your bit depth

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
      wavFile.write((byte*)queue1.readBuffer(), 256);
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
  wavFile.write((byte*)&fileSize, 4);
  wavFile.write("WAVE");

  // Write fmt subchunk
  wavFile.write("fmt ");
  wavFile.write((byte*)&fmtSize, 4);
  wavFile.write((byte*)&audioFormat, 2);
  wavFile.write((byte*)&numChannels, 2);
  wavFile.write((byte*)&sampleRate, 4);
  wavFile.write((byte*)&byteRate, 4);
  wavFile.write((byte*)&blockAlign, 2);
  wavFile.write((byte*)&bitsPerSample, 2);

  // Write data subchunk
  wavFile.write("data");
  uint32_t dataLength = wavFile.size() - 44;
  wavFile.write((byte*)&dataLength, 4);
}

void sendAudioToServer() {
  // Make sure the WAV file is closed before reading
  wavFile.close();

  // Open the file for reading
  wavFile = SD.open(filename, FILE_READ);
  if (!wavFile) {
    Serial.println("Error opening WAV file for reading");
    return;
  }

  if (client.connect(server, serverPort)) {
    Serial.println("Connected to server");

    // Send HTTP POST request
    client.print("POST /recognize HTTP/1.1\r\n");
    client.print("Host: ");
    client.print(server);
    client.print(":");
    client.println(serverPort);
    client.println("Content-Type: audio/wav");

    // Include the sample rate and sample width information in the headers
    client.print("Sample-Rate: ");
    client.println(sampleRate);

    client.print("Sample-Width: ");
    client.println(bitsPerSample);

    // Get the size of the WAV file
    uint32_t fileSize = wavFile.size();

    // Print the Content-Length header
    client.print("Content-Length: ");
    client.println(fileSize);

    client.println();  // End of headers

    // Send the WAV file data in chunks
    byte chunkBuffer[CHUNK_SIZE];
    int bytesRead;
    while ((bytesRead = wavFile.read(chunkBuffer, CHUNK_SIZE)) > 0) {
      client.write(chunkBuffer, bytesRead);
    }

    delay(1000);

    // Read server response
    while (client.available()) {
      char c = client.read();
      Serial.write(c);
    }

    // Print the server status code
    int statusCode = client.status();
    Serial.print("Server Status Code: ");
    Serial.println(statusCode);

    delay(2000);

    client.stop();
    Serial.println("\nRequest sent");
  } else {
    Serial.println("Connection to server failed");
  }

  delay(2000);


  // Close the WAV file
  wavFile.close();
}

void setup() {

  Serial.begin(115200);

  ESP8266SERIAL.begin(SERIAL_ESP8266_DEBUG_BAUD);
  WiFi.init(&ESP8266SERIAL);
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    while (true)
      ;
  }
  InitWiFi();
  delay(1000);

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
  delay(1000);

  sendAudioToServer();
  delay(2000);
}