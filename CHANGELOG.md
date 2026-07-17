# Changelog

All notable changes to Water Temperature Sensor are documented here.

The format follows [Keep a Changelog](https://keepachangelog.com/en/1.1.0/). This project uses [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

---

## [1.0.0] — "Ion" — 2026-07-18

First stable release. Everything the Atom prototype did, rebuilt to be fault-tolerant, non-blocking, and observable — plus a hardware self-test and session statistics. Full notes: [docs/releases/v1.0.0.md](docs/releases/v1.0.0.md).

### Added
- **Buzzer self-test at boot.** Two short chirps right after power-up verify the alarm hardware actually works — a dead or miswired buzzer is caught immediately instead of being discovered silently during a real over-temperature event.
- **Session min/max tracking.** The lowest and highest valid readings since power-up are tracked and appended to every serial log row (`session_min_c`, `session_max_c`), so a log capture ends with the session's extremes with no post-processing.
- **Sensor fault handling.** A disconnected or failed DS18B20 no longer displays a bogus `-127.00 C` — the LCD shows `Sensor Error!`, the serial log records an `error` row, and the alarm is forced off so a bad read can't trigger a false alert. Startup also checks `getDeviceCount()` and reports `No sensor found!` if the bus is empty.
- **Alarm hysteresis.** The buzzer arms at `ALARM_ON_C` (30 °C) but disarms only below `ALARM_OFF_C` (28 °C). Previously a reading hovering exactly at the threshold toggled the buzzer on and off every second.
- **Pulsed alarm.** While alarming, the buzzer beeps on a 500 ms cycle instead of holding a solid tone — more noticeable and less grating.
- **Serial CSV logging.** The serial monitor emits `millis,temp_c,status,session_min_c,session_max_c` rows at 9600 baud (with a header), ready to paste into a spreadsheet or feed to a plotter. Previously `Serial.begin()` was called but nothing was ever printed.
- **Degree glyph.** A custom `°` character (LCD CGRAM slot 0) so the display reads `25.50°C` instead of `25.50 C`.
- **12-bit resolution** explicitly configured (0.0625 °C steps) via `setResolution()`.
- Full repo scaffolding: README with wiring tables, CHANGELOG, ROADMAP, CONTRIBUTING, CODE_OF_CONDUCT, SECURITY, SUPPORT, docs (Architecture, Development, User Guide, Wiring, Troubleshooting), issue/PR templates, and an Arduino compile CI workflow.

### Changed
- **Non-blocking loop.** `delay(1000)` and the library's blocking conversion wait are gone. Conversions run asynchronously (`setWaitForConversion(false)`) and all timing uses `millis()`, so the buzzer pulse and future additions can't be stalled by the sensor's ~750 ms conversion time.
- **Flicker-free display.** `lcd.clear()` was called every second, which blanked the screen before each redraw and made it visibly flicker. Rows are now overwritten in place and padded to the full 16 columns so shorter values fully replace longer ones.
- Configuration consolidated into named `#define`s (`ALARM_ON_C`, `READ_INTERVAL_MS`, `BEEP_PERIOD_MS`, `LCD_ADDRESS`, …) documented in the README.
- String literals moved to flash with `F()` to save SRAM.

## [0.0.1] — "Atom" — 2026-07-15 *(pre-release)*

The original proof-of-concept. Full notes: [docs/releases/v0.0.1.md](docs/releases/v0.0.1.md).

### Added
- Initial sketch: DS18B20 read once per second via blocking `delay()`, temperature shown on a 16×2 I2C LCD, buzzer held on continuously above a fixed 30 °C threshold.
- Verified in Tinkercad simulation and on physical hardware (Arduino Uno breadboard build).
