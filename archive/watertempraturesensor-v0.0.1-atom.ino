/*
 * Water Temperature Sensor — v0.0.1 "Atom" (pre-release)
 *
 * Historical artifact: the original proof-of-concept sketch, preserved as
 * released. Superseded by the non-blocking, fault-tolerant v1.0.0 "Ion"
 * implementation in the repository root. Do not modify.
 *
 * Note: this file lives in archive/ (not the sketch root) so the Arduino IDE
 * and CI never merge or compile it alongside the current sketch.
 */

#include <OneWire.h>
#include <DallasTemperature.h>
#include <LiquidCrystal_I2C.h>

// Pin Definitions
#define ONE_WIRE_BUS 2        // Temperature sensor pin
#define BUZZER_PIN 3          // Buzzer connected to pin 3
#define TEMP_THRESHOLD 30     // Temperature threshold for buzzer

// Initialize Libraries
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
LiquidCrystal_I2C lcd(0x27, 16, 2);  // LCD with I2C address 0x27, 16 columns, 2 rows

void setup() {
  // Buzzer setup
  pinMode(BUZZER_PIN, OUTPUT);

  // LCD setup
  lcd.init();          // Initialize the LCD module
  lcd.backlight();     // Turn on the LCD backlight
  lcd.setCursor(0, 0);
  lcd.print("Initializing...");  // Display initializing message
  delay(2000);                   // Hold the message for 2 seconds

  // Temperature sensor setup
  sensors.begin();     // Start temperature sensor
  Serial.begin(9600);  // Serial monitor for debugging
}

void loop() {
  // Read temperature
  sensors.requestTemperatures();
  float temperature = sensors.getTempCByIndex(0);  // Get temperature in Celsius

  // Display temperature on LCD
  lcd.clear();               // Clear LCD screen
  lcd.setCursor(0, 0);        // First line, first column
  lcd.print("Temp: ");
  lcd.print(temperature);
  lcd.print(" C");

  // Display status and alert
  if (temperature > TEMP_THRESHOLD) {
    digitalWrite(BUZZER_PIN, HIGH);  // Activate buzzer
    lcd.setCursor(0, 1);             // Second line, first column
    lcd.print("ALERT: Temp High!");  // Display alert
  } else {
    digitalWrite(BUZZER_PIN, LOW);   // Deactivate buzzer
    lcd.setCursor(0, 1);             // Second line, first column
    lcd.print("Temp Normal");        // Display normal message
  }

  delay(1000);  // Wait 1 second before the next reading
}
