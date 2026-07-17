# Source Code — v1.0.0 "Ion"

> **Canonical source:** [`WaterTempratureSensor.ino`](../WaterTempratureSensor/WaterTempratureSensor.ino) — this page is a readable reference copy, regenerated each release. If the two ever differ, the `.ino` wins.

## Full Listing

```cpp
/*
 * Water Temperature Sensor
 *
 * Reads water temperature from a DS18B20 probe, shows it on a 16x2 I2C LCD,
 * and pulses a buzzer when the temperature crosses a configurable threshold.
 *
 * Hardware: Arduino Uno/Nano, DS18B20 (waterproof probe), 16x2 LCD with
 * I2C backpack (PCF8574 @ 0x27), active buzzer, 4.7k pull-up on the data line.
 *
 * Author: Sufiyan Aasim (@SufiyanAasim)
 * Version: 1.0.0 "Ion"
 * License: MIT
 */

#include <OneWire.h>
#include <DallasTemperature.h>
#include <LiquidCrystal_I2C.h>

// ---------- Pins & hardware ----------
#define ONE_WIRE_BUS     2      // DS18B20 data pin
#define BUZZER_PIN       3      // Active buzzer (+) pin
#define LCD_ADDRESS      0x27   // I2C address of the LCD backpack (some boards use 0x3F)
#define LCD_COLS         16
#define LCD_ROWS         2

// ---------- Behaviour ----------
#define ALARM_ON_C       30.0   // Buzzer arms at or above this temperature...
#define ALARM_OFF_C      28.0   // ...and disarms only below this (hysteresis stops chatter at the edge)
#define READ_INTERVAL_MS 1000UL // How often a new reading is requested
#define BEEP_PERIOD_MS   500UL  // Buzzer on/off pulse period while the alarm is active
#define SENSOR_BITS      12     // DS18B20 resolution: 9..12 bits (12 = 0.0625 C steps)

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
LiquidCrystal_I2C lcd(LCD_ADDRESS, LCD_COLS, LCD_ROWS);

// Small degree symbol for the LCD (slot 0)
byte degreeGlyph[8] = {
  0b00110,
  0b01001,
  0b01001,
  0b00110,
  0b00000,
  0b00000,
  0b00000,
  0b00000
};

unsigned long lastRequestMs = 0;    // When the current conversion was started
unsigned long conversionMs  = 0;    // How long a conversion takes at SENSOR_BITS
bool conversionPending      = false;
bool alarmActive            = false;
bool buzzerOn               = false;
unsigned long lastBeepMs    = 0;
float sessionMinC           = NAN;  // Lowest valid reading since power-up
float sessionMaxC           = NAN;  // Highest valid reading since power-up

void setup() {
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);

  // Buzzer self-test: two short chirps at boot, so a dead or miswired buzzer
  // is caught immediately instead of discovered during a real over-temp event
  for (int i = 0; i < 2; i++) {
    digitalWrite(BUZZER_PIN, HIGH);
    delay(80);
    digitalWrite(BUZZER_PIN, LOW);
    delay(120);
  }

  Serial.begin(9600);

  lcd.init();
  lcd.backlight();
  lcd.createChar(0, degreeGlyph);
  lcd.setCursor(0, 0);
  lcd.print(F("Water Temp"));
  lcd.setCursor(0, 1);
  lcd.print(F("Starting..."));

  sensors.begin();
  sensors.setResolution(SENSOR_BITS);
  // Non-blocking mode: requestTemperatures() returns immediately and we poll
  // after the conversion time instead of stalling the whole loop.
  sensors.setWaitForConversion(false);
  conversionMs = sensors.millisToWaitForConversion(SENSOR_BITS);

  if (sensors.getDeviceCount() == 0) {
    Serial.println(F("ERROR: no DS18B20 found on the bus"));
    showStatus(F("No sensor found!"));
  }

  Serial.println(F("millis,temp_c,status,session_min_c,session_max_c"));

  // Kick off the first conversion
  sensors.requestTemperatures();
  lastRequestMs = millis();
  conversionPending = true;
}

void loop() {
  unsigned long now = millis();

  // Start a new conversion once per interval
  if (!conversionPending && now - lastRequestMs >= READ_INTERVAL_MS) {
    sensors.requestTemperatures();
    lastRequestMs = now;
    conversionPending = true;
  }

  // Pick up the result once the sensor has had time to convert
  if (conversionPending && now - lastRequestMs >= conversionMs) {
    conversionPending = false;
    float tempC = sensors.getTempCByIndex(0);

    if (tempC == DEVICE_DISCONNECTED_C) {
      setAlarm(false);
      showTemperature(NAN);
      showStatus(F("Sensor Error!   "));
      Serial.print(now);
      Serial.println(F(",,error,,"));
    } else {
      // Track session extremes (valid readings only)
      if (isnan(sessionMinC) || tempC < sessionMinC) sessionMinC = tempC;
      if (isnan(sessionMaxC) || tempC > sessionMaxC) sessionMaxC = tempC;

      // Hysteresis: arm at ALARM_ON_C, disarm only below ALARM_OFF_C
      if (tempC >= ALARM_ON_C)       setAlarm(true);
      else if (tempC < ALARM_OFF_C)  setAlarm(false);

      showTemperature(tempC);
      showStatus(alarmActive ? F("ALERT: Temp High") : F("Temp Normal     "));

      Serial.print(now);
      Serial.print(',');
      Serial.print(tempC, 2);
      Serial.print(',');
      Serial.print(alarmActive ? F("alert") : F("normal"));
      Serial.print(',');
      Serial.print(sessionMinC, 2);
      Serial.print(',');
      Serial.println(sessionMaxC, 2);
    }
  }

  // Pulse the buzzer while the alarm is active (intermittent beep, not a solid tone)
  if (alarmActive && now - lastBeepMs >= BEEP_PERIOD_MS) {
    lastBeepMs = now;
    buzzerOn = !buzzerOn;
    digitalWrite(BUZZER_PIN, buzzerOn ? HIGH : LOW);
  }
}

void setAlarm(bool active) {
  if (alarmActive == active) return;
  alarmActive = active;
  buzzerOn = active;
  lastBeepMs = millis();
  digitalWrite(BUZZER_PIN, active ? HIGH : LOW);
}

// Rewrites row 0 in place (no lcd.clear(), so the display never flickers)
void showTemperature(float tempC) {
  lcd.setCursor(0, 0);
  int written = lcd.print(F("Temp: "));
  if (isnan(tempC)) {
    written += lcd.print(F("--.--"));
  } else {
    written += lcd.print(tempC, 2);
  }
  written += lcd.write(byte(0));  // degree glyph
  written += lcd.print(F("C"));
  // Pad the rest of the row so shorter values fully overwrite longer ones
  while (written < LCD_COLS) written += lcd.print(' ');
}

// Rewrites row 1 in place; pass 16-char strings so the row is fully overwritten
void showStatus(const __FlashStringHelper *msg) {
  lcd.setCursor(0, 1);
  lcd.print(msg);
}
```

## Walkthrough

| Section | What it does |
|---|---|
| `#define` block | Every tunable — pins, LCD address, thresholds, timing. Documented in the [README configuration table](../README.md#%EF%B8%8F-configuration). |
| `setup()` | Buzzer self-test (two chirps), LCD init + splash, sensor init at 12-bit with async conversions, empty-bus check, CSV header, first conversion kicked off. |
| `loop()` | Three independent `millis()`-based jobs: start a conversion each second, collect + validate the result when ready, pulse the buzzer while alarming. No `delay()` anywhere. |
| `setAlarm()` | Alarm state transitions — called with hysteresis (`ALARM_ON_C` arm / `ALARM_OFF_C` release) so the buzzer never chatters. |
| `showTemperature()` / `showStatus()` | Flicker-free display: rows overwritten in place and padded to 16 columns; custom `°` glyph from CGRAM slot 0. |

Design rationale and the alarm state machine: [Architecture](Architecture.md). Historical v0.0.1 "Atom" source: [`archive/`](../archive/watertempraturesensor_v0_0_1_atom/watertempraturesensor_v0_0_1_atom.ino).
