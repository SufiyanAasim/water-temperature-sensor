# Hardware Requirements

Everything needed to build the device, with exact specifications.

## Bill of Materials

| # | Component | Specification | Qty | Notes |
|---|---|---|---|---|
| 1 | Arduino Uno R3 | ATmega328P, 5 V logic, 16 MHz | 1 | Nano also works (same wiring; CI-compiled) |
| 2 | DS18B20 temperature sensor | Waterproof probe, −55…+125 °C, ±0.5 °C (−10…+85 °C), OneWire | 1 | TO-92 bare version works for dry testing only |
| 3 | 16×2 character LCD | HD44780-compatible with **I2C backpack** (PCF8574) | 1 | Backpack address `0x27` (some batches `0x3F`) |
| 4 | Active buzzer | 5 V, built-in oscillator, ~30 mA | 1 | *Active* required — sketch drives plain HIGH/LOW |
| 5 | Resistor | 4.7 kΩ, ¼ W | 1 | OneWire pull-up, DATA → 5 V — **not optional** |
| 6 | Breadboard | Half-size or larger | 1 | |
| 7 | Jumper wires | Male–male + male–female | ~12 | Female needed for probe leads / LCD header |
| 8 | USB cable | USB-A → USB-B (Uno) | 1 | Power + programming + serial log |

## Electrical Requirements

| Parameter | Value |
|---|---|
| Supply | 5 V via USB (or 7–12 V barrel jack) |
| Estimated current draw | ≈ 60–100 mA total — LCD + backlight ~25 mA, DS18B20 ~1.5 mA, buzzer ~30 mA (pulsed), MCU ~20 mA |
| Power budget | Comfortably within a standard 500 mA USB port |
| Pins used | D2 (OneWire), D3 (buzzer), A4/SDA + A5/SCL (I2C), 5V, GND |

## Software Requirements

| Requirement | Version |
|---|---|
| Arduino IDE | 1.8+ or 2.x (or `arduino-cli`) |
| Board package | Arduino AVR Boards |
| `OneWire` library | 2.3+ |
| `DallasTemperature` library | 3.9+ / 4.x |
| `LiquidCrystal I2C` library | 1.1+ |

## Assembly

Full hookup tables, breadboard layout, and build photos: [Wiring Guide](Wiring.md). Common build problems: [Troubleshooting](../troubleshooting/Troubleshooting.md).
