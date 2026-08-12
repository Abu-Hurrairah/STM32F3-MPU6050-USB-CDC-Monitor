# Pinout and Configuration

This file records the relevant configuration found in `original_source/GetSensorValues1.ioc` and the MPU6050 driver.

## MCU

```text
STM32F303VCT6
LQFP100
SYSCLK: 48 MHz
HCLK:   48 MHz
USB:    48 MHz
```

## I2C1

```text
Mode: Fast
SCL: PB6
SDA: PB7
```

The original CubeMX labels are:

```text
PB6 -> I2C1_SCL [LSM303DLHC_SCL]
PB7 -> I2C1_SDA [LSM303DLHC_SDA]
```

The MPU6050 driver uses the same `I2C1` handle (`hi2c1`).

## MPU6050

```text
7-bit address: 0x68
STM32 HAL address macro: 0xD0
WHO_AM_I expected: 0x68
Data-ready input: PB5
Accelerometer: ±2 g
Gyroscope: ±250 °/s
Sample-rate divider: 39
Source comment: 200 Hz
```

## USB

```text
PA11 -> USB_DM
PA12 -> USB_DP
USB clock -> 48 MHz
```

## USART2

The recovered `.ioc` configures:

```text
USART2 baud rate: 115200
```

This is separate from USB CDC. A terminal's displayed COM-port baud setting does not represent the physical USB bus speed.
