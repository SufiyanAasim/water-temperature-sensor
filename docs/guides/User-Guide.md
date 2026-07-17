# User Guide

How to operate the Water Temperature Sensor once it's built and flashed.

## Startup

On power-up the buzzer chirps **twice** — this is the alarm self-test. If you don't hear it, the buzzer is dead or miswired; fix that before trusting the device (see [Troubleshooting](../troubleshooting/Troubleshooting.md)).

The LCD shows:

```
Water Temp
Starting...
```

If no DS18B20 is detected on the bus, the second row changes to `No sensor found!` — check the wiring and the 4.7 kΩ pull-up.

## Normal Operation

The display refreshes about once per second:

```
Temp: 25.50°C
Temp Normal
```

| Row | Meaning |
|---|---|
| `Temp: 25.50°C` | Latest reading, 12-bit resolution (0.0625 °C steps) |
| `Temp Normal` | Below the alarm threshold |
| `ALERT: Temp High` | At/above 30 °C — buzzer pulses |
| `Sensor Error!` | Probe disconnected or read failed |

## The Alarm

- **Arms** when the reading reaches **30 °C** (`ALARM_ON_C`)
- **Disarms** only when it falls **below 28 °C** (`ALARM_OFF_C`)

The 2 °C gap (hysteresis) means water hovering right at 30 °C won't make the buzzer stutter on and off. While armed, the buzzer pulses on a 500 ms cycle.

During a sensor error the alarm is forced off — the device never alarms on bad data.

## Serial Log

Open the Arduino IDE Serial Monitor at **9600 baud**. Output is CSV:

```
millis,temp_c,status,session_min_c,session_max_c
1023,25.50,normal,25.44,25.56
2047,30.12,alert,25.44,30.12
3071,,error,,
```

- `millis` — milliseconds since power-up
- `temp_c` — reading in Celsius (empty on error)
- `status` — `normal`, `alert`, or `error`
- `session_min_c` / `session_max_c` — lowest and highest valid readings since power-up; the last captured row always holds the whole session's extremes

Copy-paste into a spreadsheet, or use the IDE's Serial Plotter for a live graph of the second column.

## Changing the Thresholds

Edit the `#define`s at the top of [`WaterTempratureSensor.ino`](../../WaterTempratureSensor/WaterTempratureSensor.ino) and re-upload. Keep `ALARM_OFF_C` a degree or two below `ALARM_ON_C` — setting them equal removes the hysteresis and brings back alarm chatter. The full table of tunables is in the [README](../../README.md#%EF%B8%8F-configuration).
