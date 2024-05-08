#include <ThingsBoard.h>
#include <WiFi.h>
#include <ArduinoOTA.h>
#include <ArduinoHttpClient.h>

#include <MFRC522.h>
#include <SPI.h>
#include <Servo.h>

constexpr char WIFI_SSID[] = "CSD";               // Define WiFi SSID
constexpr char WIFI_PASSWORD[] = "csd@NITK2014";  // Define WiFi password

constexpr char TOKEN[] = "m7YJB0Xce8pqoC5vQf5W";

constexpr char THINGSBOARD_SERVER[] = "10.14.0.205";  // Define ThingsBoard server IP address
constexpr uint16_t THINGSBOARD_PORT = 1883U;          // Define ThingsBoard server port
constexpr uint32_t MAX_MESSAGE_SIZE = 256U;           // Define maximum message size
constexpr uint32_t SERIAL_DEBUG_BAUD = 115200U;

constexpr const char RFID_KEY[]  = "charArray";
constexpr const char RFID_AUTH_STATUS[] = "access";
constexpr const char RED_LED_KEY[] = "red";             // Define the attribute key for the red LED
constexpr const char GREEN_LED_KEY[] = "green";         // Define the attribute key for the green LED

constexpr const char RPC_SERVO_METHOD[] = "servo";             // Define the RPC method for setting switch state
constexpr const char RPC_SERVO_RESPONSE_KEY[] = "servo_response";  // Define the key for RPC response

WiFiClient espClient;
ThingsBoard tb(espClient, MAX_MESSAGE_SIZE);

uint8_t status = WL_IDLE_STATUS;  // the Wifi radio's status
bool subscribed = false;
bool requestedShared = false;
int msg = 0;

char *BASE_URL = "/api/v1";   // Define base URL for API requests
char *ENDPOINT = "firmware";  // Define endpoint for firmware updates
char PATH[256];               // Define array to store the path for firmware updates

constexpr const char FW_TITLE_KEY[] = "FW_TITLE";
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

#define SS_PIN 17   // Define the slave select pin for MFRC522
#define RST_PIN 10  // Define the reset pin for MFRC522

#define WIFI_STATUS_LED LED_BUILTIN  // Define the built-in LED pin for WiFi status

#define GREEN_LED 6   // Define the pin for the green LED
#define RED_LED 7     // Define the pin for the red LED
#define BUZZER_PIN 8  // Define the pin for the buzzer
#define SERVO_PIN 9   // Define the pin for the servo motor

#define BUZZER_VOLUME 10  // Adjust this value to control the buzzer BUZZER_VOLUME

#define CLOSE_POS 180  // Define the closed position of the servo motor
#define OPEN_POS 40    // Define the open position of the servo motor

#define DEBOUNCE_TIME 2000  // Define the debounce time for actuation
#define BLINK_INTERVAL 250  // Define the blink interval for LED and buzzer

#define CARD1 "23 CB 66 0E"  // Define the UID of the first authorized card
#define CARD2 "A3 EF 6C A6"  // 33 25 06 80  Define the UID of the second authorized card

Servo myservo;  // Create a servo object

MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance.

char charArray[15];

int pos = 0;  // Declare variable for servo motor position
int switch_state = 0;
bool buzzerState = false;  // Initialize buzzer state

void InitWiFi() {
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
  status = WiFi.status();
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

void openDoor() {
  for (pos = CLOSE_POS; pos >= OPEN_POS; pos -= 10) {
    myservo.write(pos);
    delay(1);
  }
}

void closeDoor() {
  for (pos = OPEN_POS; pos <= CLOSE_POS; pos += 10) {
    myservo.write(pos);
    delay(1);
  }
}

void actuate() {
  Serial.println("Actuated");

  unsigned long startMillis = millis();
  unsigned long previousMillis = millis();

  unsigned long interval = BLINK_INTERVAL;
  unsigned long duration = DEBOUNCE_TIME;

  while (previousMillis - startMillis < duration) {
    if (millis() - previousMillis >= interval) {
      previousMillis = millis();
      digitalWrite(RED_LED, !digitalRead(RED_LED));
      if (buzzerState == false) {
        analogWrite(BUZZER_PIN, BUZZER_VOLUME);
        buzzerState = true;
      } else {
        analogWrite(BUZZER_PIN, 0);
        buzzerState = false;
      }
    }
  }
  digitalWrite(RED_LED, LOW);
  analogWrite(BUZZER_PIN, 0);
}

RPC_Response setServoSwitchState(RPC_Data &data) {
  Serial.println("Received Servo State");
  switch_state = data;
  Serial.println("Servo State Change:");
  Serial.println(switch_state);
  return RPC_Response(RPC_SERVO_RESPONSE_KEY, switch_state);
}

const std::array<RPC_Callback, 1U> callbacks = {
  RPC_Callback{ RPC_SERVO_METHOD, setServoSwitchState }
};

void setup() {
  // Initalize serial connection for debugging
  Serial.begin(SERIAL_DEBUG_BAUD);
  delay(1000);

  myservo.attach(SERVO_PIN);

  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(WIFI_STATUS_LED, OUTPUT);

  digitalWrite(RED_LED, LOW);
  digitalWrite(GREEN_LED, LOW);
  myservo.write(CLOSE_POS);

  digitalWrite(WIFI_STATUS_LED, LOW);

  SPI.begin();         // Initiate  SPI bus
  mfrc522.PCD_Init();  // Initiate MFRC522

  InitWiFi();

  Serial.println("Approximate your card to the reader...");
  Serial.println();
}

void loop() {
  delay(1000);

  if (!reconnect()) {
    return;
  }

  if (!tb.connected()) {
    // Reconnect to the ThingsBoard server,
    // if a connection was disrupted or has not yet been established
    Serial.printf("Connecting to: (%s) with token (%s)\n", THINGSBOARD_SERVER, TOKEN);
    digitalWrite(WIFI_STATUS_LED, LOW);
    if (!tb.connect(THINGSBOARD_SERVER, TOKEN, THINGSBOARD_PORT)) {

      Serial.println("Failed to connect");
      return;
    }
    subscribed = false;
  }

  if (!subscribed) {                           // Check if not already subscribed for RPC
    Serial.println("Subscribing for RPC...");  // Print a message indicating subscription for RPC

    // Perform a subscription. All consequent data processing will happen in
    // processTemperatureChange() and processSwitchChange() functions,
    // as denoted by callbacks array.
    if (!tb.RPC_Subscribe(callbacks.cbegin(), callbacks.cend())) {  // Subscribe for RPC with provided callback functions
      Serial.println("Failed to subscribe for RPC");                // Print a message indicating failure to subscribe for RPC
      return;                                                       // Exit the function if subscription fails
    }
    Serial.println("Subscribe done");     // Print a message indicating successful subscription
    digitalWrite(WIFI_STATUS_LED, HIGH);  // Turn on the WiFi status LED
    subscribed = true;                    // Update the subscription status to true
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

  // Look for new cards
  if (mfrc522.PICC_IsNewCardPresent()) {  // Check if a new RFID card is present
    //Select one of the cards
    if (mfrc522.PICC_ReadCardSerial()) {  // Read the serial number of the RFID card
      //Show UID on serial monitor
      Serial.print("UID tag :");                                             // Print the message "UID tag :" to indicate that the following is the UID
      String content = "";                                                   // Initialize an empty string to store the UID content
      byte letter;                                                           // Declare a byte variable to store each byte of the UID
      for (byte i = 0; i < mfrc522.uid.size; i++) {                          // Iterate over each byte of the UID
        Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");            // Print a leading space if the byte is less than 0x10 (single digit hexadecimal)
        Serial.print(mfrc522.uid.uidByte[i], HEX);                           // Print the byte in hexadecimal format
        content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));  // Append a space to the content string if necessary
        content.concat(String(mfrc522.uid.uidByte[i], HEX));                 // Append the byte to the content string in hexadecimal format
      }
      Serial.println();            // Print an empty line to add a line break in the serial monitor
      Serial.print("Message : ");  // Print the message header "Message : " in the serial monitor
      content.toUpperCase();

      // String arduinoString = "Hello";
      charArray[content.length() + 1];  // +1 for null terminator
      content.toCharArray(charArray, sizeof(charArray));
      Serial.println(charArray);

      tb.sendTelemetryString(RFID_KEY, charArray);
    }
  }

  if (switch_state == 40) {  //change here the UID of the card/cards that you want to give access
    switch_state = 0;
    tb.sendTelemetryString(RFID_AUTH_STATUS, "Authorized access");
    tb.sendAttributeBool(GREEN_LED_KEY, true);
    Serial.println("Authorized access");  // Print "Authorized access" message to serial monitor
    Serial.println();                     // Print an empty line for formatting
    // tb.sendAttributeBool(GREEN_LED_KEY, true);         // Send attribute to turn on the green LED indicator
    // tb.sendTelemetryString(RFID_KEY, "Door is Open");  // Send telemetry indicating that the door is open
    digitalWrite(GREEN_LED, HIGH);  // Turn on the green LED
    openDoor();                     // Call the function to open the door
    delay(3000);                    // Delay for 3 seconds
    closeDoor();                    // Call the function to close the door
    // switch_state = 0;
  } else if (switch_state == 30) {
    switch_state = 0;
    tb.sendTelemetryString(RFID_AUTH_STATUS, "Access denied");
    tb.sendAttributeBool(RED_LED_KEY, true);
    Serial.println("Access denied");  // Print "Access denied" message to serial monitor
    // digitalWrite(RED_LED, HIGH);
    // delay(1000);
    // digitalWrite(RED_LED, LOW);
    actuate();  // Call the actuate function to perform additional actions for access denial
  } else {
    switch_state = 0;
    tb.sendAttributeBool(GREEN_LED_KEY, false);
    tb.sendAttributeBool(RED_LED_KEY, false);
    tb.sendTelemetryString(RFID_AUTH_STATUS, "Door is Close");  // Send telemetry indicating the door is closed
  }

  digitalWrite(RED_LED, LOW);    // Turn off the red LED
  digitalWrite(GREEN_LED, LOW);  // Turn off the green LED

  tb.loop();
}
