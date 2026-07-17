# Development

## Environment

- **Arduino IDE** 2.x (or `arduino-cli`)
- Board package: **Arduino AVR Boards** (Uno / Nano)
- Libraries (Library Manager):
  - `OneWire` (Paul Stoffregen)
  - `DallasTemperature` (Miles Burton)
  - `LiquidCrystal I2C` (Frank de Brabander / johnrickman)

## Building

**Arduino IDE:** open [`WaterTempratureSensor.ino`](../WaterTempratureSensor/WaterTempratureSensor.ino), select *Arduino Uno* and your port, Upload.

**arduino-cli:**

```bash
arduino-cli core install arduino:avr
arduino-cli lib install OneWire DallasTemperature "LiquidCrystal I2C"
arduino-cli compile --fqbn arduino:avr:uno WaterTempratureSensor
arduino-cli upload  --fqbn arduino:avr:uno -p <PORT> WaterTempratureSensor
```

## Simulation

The circuit can be developed without hardware in [Tinkercad Circuits](https://www.tinkercad.com/circuits) — the original design was verified there before assembly (layout reference: [Wiring Guide](hardware/Wiring.md#breadboard-build)). Note Tinkercad's serial monitor is fixed to the simulation speed, so timing-sensitive behaviour (beep cadence, conversion waits) is best confirmed on real hardware.

## Testing a Change

There is no unit-test harness (the sketch is a single hardware-bound loop); verification is behavioural:

1. **Compile clean** for Uno and Nano (CI enforces this on every push — [workflow](../.github/workflows/build.yml)).
2. **Boot** — expect two self-test chirps, then `Water Temp / Starting...`.
3. **Normal path** — reading updates every second, no flicker, serial rows well-formed.
4. **Alarm path** — warm the probe past 30 °C: `ALERT: Temp High` + pulsed buzzer; cool below 28 °C: alarm releases (not at 29 °C — that's the hysteresis working).
5. **Fault path** — pull the DATA wire mid-run: `Sensor Error!`, `error` serial row, alarm forced off. Reconnect: recovery without reset.

## Code Guidelines

See [CONTRIBUTING.md](../CONTRIBUTING.md) — the short version:

- No `delay()` in the loop; all timing via `millis()`
- New tunables go in the `#define` block + README configuration table
- String literals wrapped in `F()`
- Every sensor interaction must handle `DEVICE_DISCONNECTED_C`

## Release Process

See [RELEASE.md](../RELEASE.md) — it covers versioning and codenames, the
pre-release checklist, which files to update, checksum refresh, and tagging.
