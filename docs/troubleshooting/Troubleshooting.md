# Troubleshooting

## LCD is blank or shows solid blocks

1. **Contrast** — turn the blue trimmer on the I2C backpack until characters appear.
2. **Address** — many backpacks are `0x3F`, not `0x27`. Run an [I2C scanner sketch](https://playground.arduino.cc/Main/I2cScanner/) and set `LCD_ADDRESS` accordingly.
3. **Wiring** — SDA→A4, SCL→A5 on Uno/Nano (easy to swap by accident).

## Display shows `Sensor Error!` (or serial logs `error`)

The sketch received `DEVICE_DISCONNECTED_C` (-127) from the library:

1. **Missing pull-up** — the 4.7 kΩ resistor between DATA and 5 V is required. This is the #1 cause.
2. **Loose data wire** — reseat the D2 connection.
3. **Swapped wires** — on waterproof probes: red = 5 V, black = GND, yellow = data. Reversed VCC/GND can permanently damage the probe (it usually gets hot — unplug immediately).
4. **Counterfeit DS18B20** — some clones fail at 12-bit resolution; try `SENSOR_BITS 9`.

## `No sensor found!` at startup

Same causes as above, but present from boot — the bus enumeration found zero devices. Check the pull-up and wiring first.

## Buzzer never sounds

1. **Listen for the boot self-test** — the buzzer chirps twice on power-up. No chirps means the buzzer itself (or its wiring) is the problem, not the temperature logic.
2. Heat the probe past 30 °C (hold it in your hand for a while, or warm water) and watch for `ALERT: Temp High`.
3. If the LCD shows the alert but there's no sound, check buzzer polarity and that + is on D3.
4. **Passive buzzer?** The sketch drives an *active* buzzer with plain HIGH/LOW. A passive one needs `tone(BUZZER_PIN, 2000)` instead of `digitalWrite`.

## Buzzer stutters on/off around the threshold

Shouldn't happen with default settings — hysteresis disarms only below `ALARM_OFF_C` (28 °C). If you edited the thresholds, make sure `ALARM_OFF_C` is at least ~1 °C below `ALARM_ON_C`.

## Readings look stuck or update slowly

- The reading refreshes every `READ_INTERVAL_MS` (1 s default) — the DS18B20's 12-bit conversion itself takes ~750 ms, so don't set the interval below ~800 ms at 12 bits.
- For faster updates, lower `SENSOR_BITS` (9-bit converts in ~94 ms at 0.5 °C steps).

## Serial monitor shows garbage

Set the monitor to **9600 baud** to match `Serial.begin(9600)`.
