# Roadmap

Planned direction for Water Temperature Sensor. Items are aspirational, not commitments — PRs toward any of them are welcome.

## v1.x (near term)

- [ ] **Serial configuration** — set `ALARM_ON_C` / `ALARM_OFF_C` over the serial monitor at runtime, persisted to EEPROM
- [ ] **Fahrenheit mode** — compile-time (or serial) toggle for °F display
- [ ] **Mute button** — a push-button to silence an active alarm without changing thresholds

## v2.0 (larger)

- [ ] **Multi-probe support** — read every DS18B20 on the bus and cycle/alternate the display
- [ ] **SD-card logging** — timestamped CSV to SD, matching the existing serial format
- [ ] **RTC integration** — real timestamps instead of `millis()` in logs

## Ideas / Unscheduled

- ESP32 port with a small web dashboard
- Low-temperature (freeze warning) alarm alongside the high alarm
- Trend arrow on the LCD (rising / falling / steady)

See the [Changelog](CHANGELOG.md) for what has already shipped.
