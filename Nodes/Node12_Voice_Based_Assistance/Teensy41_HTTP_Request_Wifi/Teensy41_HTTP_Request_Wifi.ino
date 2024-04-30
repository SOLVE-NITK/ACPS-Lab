#include <Arduino.h>
#include <WiFiEsp.h>
#include <WiFiEspClient.h>

constexpr char WIFI_SSID[] = "CSD";
constexpr char WIFI_PASSWORD[] = "csd@NITK2014";
char server[] = "10.100.80.26";
const int serverPort = 9876;

constexpr uint32_t SERIAL_ESP8266_DEBUG_BAUD = 115200U;

#define ESP8266SERIAL Serial1

WiFiEspClient client;

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
}

void loop() {
  if (!reconnect()) {
    return;
  }


  if (client.connect(server, serverPort)) {
    Serial.println("Connected to server");

    // Prepare POST data
    String postData = "key1=value1&key2=value2";  // Replace with your data

    // Send HTTP POST request
    client.print("POST /recognize HTTP/1.1\r\n");
    client.print("Host: ");
    client.print(server);
    client.print(":");
    client.println(serverPort);
    client.println("Content-Type: application/wav");
    client.print("Content-Length: ");
    client.println(postData.length());
    client.println();
    client.println(postData);

    delay(1000);

    // Read server response
    while (client.available()) {
      char c = client.read();
      Serial.write(c);
    }

    client.stop();
    Serial.println("\nRequest sent");
  } else {
    Serial.println("Connection to server failed");
  }

  delay(5000);  // Wait before sending the next request
}
