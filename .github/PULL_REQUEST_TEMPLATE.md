## Summary

<!-- What does this PR change, and why? -->

## Checklist

- [ ] Compiles for the Uno (`arduino-cli compile --fqbn arduino:avr:uno .`) — CI will verify too
- [ ] No `delay()` introduced into the loop; timing stays `millis()`-based
- [ ] Sensor fault paths (`DEVICE_DISCONNECTED_C`, empty bus) still handled
- [ ] New tunables added to the `#define` block and the README configuration table
- [ ] CHANGELOG.md updated
- [ ] Tested on real hardware (state board below) — or explain why not

## Hardware tested on

<!-- e.g. Arduino Uno R3, waterproof DS18B20, 0x27 LCD -->
