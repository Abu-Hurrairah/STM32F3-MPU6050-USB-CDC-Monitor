# Architecture

The STM32 performs a 14-byte burst read starting at MPU6050 register `0x3B`:

```text
Accelerometer X/Y/Z
Temperature
Gyroscope X/Y/Z
```

Each high/low byte pair becomes a signed 16-bit value.

The supplied configuration uses:

```text
Accelerometer: ±2 g
Gyroscope: ±250 degrees/second
```

Conversions:

```text
acceleration_g = raw / 16384
gyro_dps       = raw / 131
temperature_C  = raw / 340 + 36.53
```

The application sends formatted text through `CDC_Transmit_FS()`.
