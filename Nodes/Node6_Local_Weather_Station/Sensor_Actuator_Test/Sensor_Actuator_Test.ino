#include <Adafruit_BMP280.h>
#include <DHT.h>
#include <Wire.h>
#include <DS3231.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <WiFiNINA.h>
#include <NTPClient.h>

// Replace with your network credentials
const char* ssid = "CSD";
const char* password = "csd@NITK2014";

// NTP Server settings
const char* ntpServer = "10.100.80.26";
const long gmtOffsetInSeconds = 19800;  // Your GMT offset in seconds

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C  // Address of the OLED display

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define DHTPIN 2
#define DHTTYPE DHT21

// Create instances of necessary libraries
WiFiClient wifiClient;
WiFiUDP udp;                                               // Create a UDP instance
NTPClient timeClient(udp, ntpServer, gmtOffsetInSeconds);  // Use UDP instance in NTPClient constructor
Adafruit_BMP280 bmp;                                       // I2C
DHT dht(DHTPIN, DHTTYPE);
RTClib myRTC;
DS3231 myRTC2;

float temp, humid, hI, pressure, alti;
int year, month, date, day, hour, minute, second;

const char* daysOfWeek[] = { "", "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday" };
const char* monthsOfYear[] = { "", "January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December" };

void updateRTCFromNTP() {
  timeClient.begin();
  timeClient.update();

  unsigned long epochTime = timeClient.getEpochTime();
  myRTC2.setEpoch(epochTime);

  Serial.print("Updated RTC with NTP time: ");
  Serial.println(epochTime);
}

void setup() {
  Serial.begin(115200);
  while (!Serial) delay(100);  // wait for native usb

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Initialize DS3231 RTC
  Wire.begin();

  myRTC2.setClockMode(false);  // set to 24h
  // Update RTC with NTP time
  updateRTCFromNTP();

  bmp.begin(BMP280_ADDRESS_ALT, BMP280_CHIPID);
  dht.begin();

  display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
  display.setRotation(2);  // Rotate 180 degrees

  /* Default settings from datasheet. */
  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                  Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                  Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */
}

void loop() {

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
  display.println(F("°C "));

  display.print(F("Heat index: "));
  display.print(hI, 1);
  display.println(F("°C "));

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
  delay(1000);
}
