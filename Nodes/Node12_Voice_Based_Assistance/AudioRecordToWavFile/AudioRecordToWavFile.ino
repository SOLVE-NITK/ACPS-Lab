#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>

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
const long interval = 5000;

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
  uint32_t fmtSize = 16;
  wavFile.write((byte*)&fmtSize, 4);
  uint16_t audioFormat = 1;  // PCM format
  wavFile.write((byte*)&audioFormat, 2);
  uint16_t numChannels = 1;  // Mono
  wavFile.write((byte*)&numChannels, 2);
  uint32_t sampleRate = 44100;  // Change to your sample rate
  wavFile.write((byte*)&sampleRate, 4);
  uint32_t byteRate = sampleRate * numChannels * 2;  // 16-bit audio
  wavFile.write((byte*)&byteRate, 4);
  uint16_t blockAlign = numChannels * 2;
  wavFile.write((byte*)&blockAlign, 2);
  uint16_t bitsPerSample = 16;  // Change to your bit depth
  wavFile.write((byte*)&bitsPerSample, 2);

  // Write data subchunk
  wavFile.write("data");
  uint32_t dataLength = wavFile.size() - 44;
  wavFile.write((byte*)&dataLength, 4);
}

void setup() {
  AudioMemory(60);

  if (!SD.begin(SDCARD_CS_PIN)) {
    while (1) {
      Serial.println("Unable to access the SD card");
      delay(500);
    }
  }
}

void loop() {
  startRecording();
  unsigned long currentMillis = millis();
  previousMillis = millis();
  while (previousMillis - currentMillis <= interval) {
    previousMillis = millis();
    continueRecording();
  }
  stopRecording();
  delay(5000);
}