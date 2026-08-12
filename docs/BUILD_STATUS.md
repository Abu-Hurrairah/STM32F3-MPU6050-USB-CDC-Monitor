# Original Build Status

`GetSensorValues1` archived STM32CubeIDE build: **0 errors, 4 warnings**.

The warnings were `printf` implicit-declaration warnings in `MPU6050.c` and `main.c`; adding `<stdio.h>` resolves the declaration warning. The USB CDC layer in this repository is reconstructed because the `Communication4` source folder was absent from the archive.
