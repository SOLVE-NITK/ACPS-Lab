#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_MLX90614.h>

#define SSD1306_I2C_ADDRESS 0x3C
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define OLED_RESET -1

// Pin definitions for the push buttons and LEDs
#define BUTTON_CELSIUS_PIN 2
#define BUTTON_FAHRENHEIT_PIN 3
#define LED_PIN_HIGH_TEMP 4
#define LED_PIN_BELOW_THRESHOLD 5

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
Adafruit_MLX90614 mlx;

volatile boolean displayInCelsius = true;
const float thresholdTempC = 30.0;  // Adjust the threshold temperature in Celsius

void celsiusButtonISR() {
  displayInCelsius = true;
}

void fahrenheitButtonISR() {
  displayInCelsius = false;
}

void setup() {
  Serial.begin(115200);
  Wire.begin();
  mlx.begin();

  // Set the push button pins as inputs with internal pull-ups
  pinMode(BUTTON_CELSIUS_PIN, INPUT_PULLUP);
  pinMode(BUTTON_FAHRENHEIT_PIN, INPUT_PULLUP);

  // Set the LED pins as outputs
  pinMode(LED_PIN_HIGH_TEMP, OUTPUT);
  pinMode(LED_PIN_BELOW_THRESHOLD, OUTPUT);
  digitalWrite(LED_PIN_HIGH_TEMP, LOW);        // Initialize the LED as off
  digitalWrite(LED_PIN_BELOW_THRESHOLD, LOW);  // Initialize the LED as off

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
}

void loop() {
  // Read the temperatures
  float objectTempC = mlx.readObjectTempC();
  float objectTempF = mlx.readObjectTempF();
  float ambientTempC = mlx.readAmbientTempC();
  float ambientTempF = mlx.readAmbientTempF();

  // Update the display based on the value of displayInCelsius
  display.clearDisplay();
  display.setCursor(0, 0);

  display.setTextColor(SSD1306_WHITE);
  if (displayInCelsius) {
    display.setTextSize(1, 2);  // Smaller text size for the first line
    display.print("Room: ");
    display.setTextSize(2);  // Bigger text size for the first line
    display.print(ambientTempC, 1);
    display.print(" C");

  } else {
    display.setTextSize(1, 2);  // Smaller text size for the first line
    display.print("Room: ");
    display.setTextSize(2);  // Bigger text size for the first line
    display.print(ambientTempF, 1);
    display.print(" F");
  }

  display.setTextSize(2);  // Bigger text size for the second line
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 17);  // Move the cursor to the second line
  if (displayInCelsius) {
    display.setTextSize(1, 2);  // Smaller text size for the first line
    display.print("Body: ");
    display.setTextSize(2);  // Bigger text size for the first line
    display.print(objectTempC, 1);
    display.print(" C");
  } else {
    display.setTextSize(1, 2);  // Smaller text size for the first line
    display.print("Body: ");
    display.setTextSize(2);  // Bigger text size for the first line
    display.print(objectTempF, 1);
    display.print(" F");
  }

  // Check if the object temperature crosses the high threshold
  if (objectTempC > thresholdTempC) {
    digitalWrite(LED_PIN_BELOW_THRESHOLD, LOW);  // Turn off the below threshold LED
    digitalWrite(LED_PIN_HIGH_TEMP, HIGH);       // Turn on the high-temperature LED
  } else {
    digitalWrite(LED_PIN_HIGH_TEMP, LOW);        // Turn off the high-temperature LED
    digitalWrite(LED_PIN_BELOW_THRESHOLD, HIGH);  // Turn on the below threshold LED
  }

  display.display();
  delay(250);  // Adjust this delay as needed
}
