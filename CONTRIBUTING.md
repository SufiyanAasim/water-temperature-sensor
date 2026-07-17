# Contributing

Thanks for your interest in improving Water Temperature Sensor!

## How to Contribute

1. **Fork** the repository and create a branch from `main`:
   ```
   git checkout -b feat/my-improvement
   ```
2. **Make your change.** Keep the sketch self-contained — no extra source files unless a change genuinely needs one.
3. **Verify it compiles** for the Uno:
   ```
   arduino-cli compile --fqbn arduino:avr:uno .
   ```
   If you have the hardware, flash it and confirm the LCD, buzzer, and serial log all still behave.
4. **Update the docs** if behaviour changed — README configuration table, User Guide, and CHANGELOG.
5. **Open a Pull Request** using the PR template.

## Guidelines

- **No `delay()` in the loop.** The sketch is fully non-blocking (`millis()`-based); keep it that way.
- **Config stays at the top.** New tunables go in the `#define` block and get a row in the README's configuration table.
- **Flash strings.** Wrap string literals in `F()` to keep them out of SRAM.
- **Fault paths matter.** Any new sensor interaction must handle `DEVICE_DISCONNECTED_C` / missing hardware gracefully.
- Follow [Keep a Changelog](https://keepachangelog.com/) and [SemVer](https://semver.org/) for release notes.

## Reporting Bugs & Requesting Features

Use the [issue templates](.github/ISSUE_TEMPLATE/). For security concerns, see [SECURITY.md](SECURITY.md).

## Code of Conduct

By participating you agree to the [Code of Conduct](CODE_OF_CONDUCT.md).
