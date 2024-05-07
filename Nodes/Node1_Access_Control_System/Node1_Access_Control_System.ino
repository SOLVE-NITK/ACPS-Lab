#include <WiFi.h>         // Include the WiFi library
#include <MFRC522.h>      // Include the MFRC522 library
#include <SPI.h>          // Include the SPI library
#include <Servo.h>        // Include the Servo library
#include <ThingsBoard.h>  // Include ArduinoHttpClient library for making HTTP requests

constexpr char WIFI_SSID[] = "CSD";                   // Define WiFi SSID
constexpr char WIFI_PASSWORD[] = "csd@NITK2014";      // Define WiFi password
constexpr char THINGSBOARD_SERVER[] = "10.14.0.205";  // Define ThingsBoard server IP address
constexpr uint16_t THINGSBOARD_PORT = 1883U;          // Define ThingsBoard server port
constexpr uint32_t MAX_MESSAGE_SIZE = 256U;           // Define maximum message size
constexpr uint32_t SERIAL_DEBUG_BAUD = 115200U;
// #include "common.h"   // Include the common header file

char charArray[15];

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

constexpr char RFID_KEY[] = "rfid";        // Define the telemetry key for RFID data
constexpr char RED_LED_KEY[] = "red";      // Define the attribute key for the red LED
constexpr char GREEN_LED_KEY[] = "green";  // Define the attribute key for the green LED

constexpr const char RPC_SWITCH_METHOD[] = "RFID";             // Define the RPC method for setting switch state
constexpr const char RPC_SWITCH_KEY[] PROGMEM = "switch";      // Define the key for switch state in RPC data
constexpr const char RPC_RESPONSE_KEY[] = "example_response";  // Define the key for RPC response

constexpr char ACTUATOR_KEY1[] PROGMEM = "actuator1";
constexpr char ACTUATOR_KEY2[] PROGMEM = "actuator2";
constexpr const char AUTHORIZE_TELEMETRY[] PROGMEM = "access";

int status = WL_IDLE_STATUS;  // Initialize WiFi status
bool subscribed = false;      // Initialize subscription status

int pos = 0;  // Declare variable for servo motor position
int switch_state = 0;
bool buzzerState = false;  // Initialize buzzer state

constexpr char TOKEN[] PROGMEM = "m7YJB0Xce8pqoC5vQf5W";           // Define ThingsBoard token
constexpr const char LIGHT_INTENSITY_KEY[] PROGMEM = "charArray";  // Define test key

Servo myservo;  // Create a servo object

MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance.
                                   //Arduino_MQTT_Client mqttClient(espClient);
WiFiClient espClient;
ThingsBoard tb(espClient, MAX_MESSAGE_SIZE);

// @brief Initalizes WiFi connection,
// will endlessly delay until a connection has been successfully established
void InitWiFi() {

  // // Configure static IP and DNS server
  // WiFi.config(localIP, dns, gateway, subnet);
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

// RPC_Response processSwitchChange(const RPC_Data &data) {
//   Serial.println("Received the set switch method");  // Print a message indicating that the switch method was received

//   // Process data
//   switch_state = data;  // Extract the switch state from the RPC data

//   Serial.print("Example switch state: ");  // Print a message indicating the switch state
//   Serial.println(switch_state);            // Print the switch state

//   // Just an response example
//   StaticJsonDocument<JSON_OBJECT_SIZE(1)> doc;  // Create a JSON document for the response
//   doc[RPC_RESPONSE_KEY] = 22.02;                // Set the response value
//   return RPC_Response(doc);                     // Return the response
// }

// const std::array<RPC_Callback, 1U> callbacks = {
//   // Define an array of RPC callbacks with one element
//   RPC_Callback{ RPC_SWITCH_METHOD, processSwitchChange }  // The element specifies the method and the function to handle it
// };

RPC_Response setServoSwitchState(RPC_Data &data) {
  Serial.println("RECIEVED SWITCH STATE");
  switch_state = data;
  Serial.println("SWITCH STATE CHANGE:");
  Serial.print(switch_state);
  return RPC_Response("setServoSwitchValue", switch_state);
}

const std::array<RPC_Callback, 2U> callbacks = {
  RPC_Callback{ "setServoSwitchValue", setServoSwitchState },
  RPC_Callback{ RPC_SWITCH_METHOD, setServoSwitchState }
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
    // Check if not connected to ThingsBoard server
    // Reconnect to the ThingsBoard server,
    // if a connection was disrupted or has not yet been established
    Serial.print("Connecting to: ");                                 // Print a message indicating connection attempt
    Serial.print(THINGSBOARD_SERVER);                                // Print the ThingsBoard server address
    Serial.print(" with token ");                                    // Print a message indicating the authentication token
    Serial.println(TOKEN);                                           // Print the authentication token
    if (!tb.connect(THINGSBOARD_SERVER, TOKEN, THINGSBOARD_PORT)) {  // Attempt to connect to ThingsBoard server

      Serial.println("Failed to connect");  // Print a message indicating connection failure
      return;                               // Exit the function if connection fails
    }
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

      tb.sendTelemetryString(LIGHT_INTENSITY_KEY, charArray);
      delay(1000);
    }
  }


  if (switch_state == 40) {  //change here the UID of the card/cards that you want to give access

    switch_state = 0;
    tb.sendTelemetryString(AUTHORIZE_TELEMETRY, "Authorized access");
    tb.sendAttributeBool(ACTUATOR_KEY1, true);
    Serial.println("Authorized access");  // Print "Authorized access" message to serial monitor
    Serial.println();                     // Print an empty line for formatting
    // tb.sendAttributeBool(GREEN_LED_KEY, true);         // Send attribute to turn on the green LED indicator
    // tb.sendTelemetryString(RFID_KEY, "Door is Open");  // Send telemetry indicating that the door is open
    digitalWrite(GREEN_LED, HIGH);  // Turn on the green LED
    openDoor();                     // Call the function to open the door
    delay(3000);                    // Delay for 3 seconds
    closeDoor();                    // Call the function to close the door
    // switch_state = 0;
  }
    // else {
    //   switch_state = 0;
    // tb.sendAttributeBool(ACTUATOR_KEY, false);
    // tb.sendTelemetryString(AUTHORIZE_TELEMETRY, "Access denied");
  // }


  else if (switch_state == 30) {
    switch_state = 0;

    tb.sendTelemetryString(AUTHORIZE_TELEMETRY, "Access denied");
    tb.sendAttributeBool(ACTUATOR_KEY2, true);
    Serial.println("Access denied");  // Print "Access denied" message to serial monitor
    // digitalWrite(RED_LED, HIGH);
    // delay(1000);
    // digitalWrite(RED_LED, LOW);


                                      //   // tb.sendAttributeBool(RED_LED_KEY, true);             // Send attribute to turn on the red LED indicator
                                      //   // tb.sendTelemetryString(RFID_KEY, "Access Denied!");  // Send telemetry indicating access denial
    actuate();                        // Call the actuate function to perform additional actions for access denial
  }

    else {
      switch_state = 0;
    tb.sendAttributeBool(ACTUATOR_KEY1, false);
    tb.sendAttributeBool(ACTUATOR_KEY2, false);
    tb.sendTelemetryString(AUTHORIZE_TELEMETRY, "Access denied");
  }



  // Serial.println(charArray);

  // tb.sendTelemetryString(RFID_KEY, "Door is Close");  // Send telemetry indicating the door is closed
  // tb.sendAttributeBool(RED_LED_KEY, false);           // Send attribute to turn off the red LED indicator
  // tb.sendAttributeBool(GREEN_LED_KEY, false);         // Send attribute to turn off the green LED indicator
  digitalWrite(RED_LED, LOW);    // Turn off the red LED
  digitalWrite(GREEN_LED, LOW);  // Turn off the green LED

  tb.loop();
}
