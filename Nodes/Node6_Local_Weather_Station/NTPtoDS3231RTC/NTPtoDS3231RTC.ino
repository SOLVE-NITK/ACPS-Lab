#include <WiFiNINA.h>
#include <NTPClient.h>
#include <DS3231.h>
#include <Wire.h>

// Replace with your network credentials
const char* ssid = "CSD";
const char* password = "csd@NITK2014";

// NTP Server settings
const char* ntpServer = "10.100.80.26";
const long gmtOffsetInSeconds = 19800;  // Your GMT offset in seconds

// Create instances of necessary libraries
WiFiClient wifiClient;
WiFiUDP udp; // Create a UDP instance
NTPClient timeClient(udp, ntpServer, gmtOffsetInSeconds); // Use UDP instance in NTPClient constructor
RTClib rtc;
DS3231 myRTC2;

void setup() {
  Serial.begin(115200);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Initialize DS3231 RTC
  Wire.begin();

  // Update RTC with NTP time
  updateRTCFromNTP();
}

void loop() {
  // Get current time from RTC
  DateTime now = rtc.now();
  
  // Print formatted time
  Serial.print("Current time: ");
  Serial.print(now.year(), DEC);
  Serial.print('/');
  Serial.print(now.month(), DEC);
  Serial.print('/');
  Serial.print(now.day(), DEC);
  Serial.print(" ");
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.print(now.second(), DEC);
  Serial.println();

  delay(1000);  // Delay for 1 second
}

void updateRTCFromNTP() {
  timeClient.begin();
  timeClient.update();
  
  unsigned long epochTime = timeClient.getEpochTime();
  myRTC2.setEpoch(epochTime);

  Serial.print("Updated RTC with NTP time: ");
  Serial.println(epochTime);
}
