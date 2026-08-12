# Troubleshooting

## MPU6050 not detected

Check power, common ground, SDA/SCL orientation, CubeMX pin assignment, AD0 to GND, pull-up resistors, and I²C speed.

## Virtual COM Port missing

Check USB Device CDC middleware, USB pin selection, 48 MHz USB clock, data cable, correct connector, and Device Manager.

## Repeated -32768

`-32768` is the minimum signed `int16_t`. Check that the 14-byte burst read completed, the read begins at `0x3B`, high/low bytes are combined correctly, and the I²C link is stable.

## Floating values missing

Enable:

```text
-u _printf_float
```
