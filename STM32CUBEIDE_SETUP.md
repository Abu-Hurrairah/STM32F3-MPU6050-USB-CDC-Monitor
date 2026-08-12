# STM32CubeIDE Setup

1. Import/open `original_source/GetSensorValues1.ioc`.
2. Target shown by the original configuration: `STM32F303V(B-C)Tx`, LQFP100.
3. I2C1 is used for the sensor. The original configuration uses PB6/PB7.
4. USB uses PA11/PA12 and a 48 MHz USB clock.
5. Enable `USB_DEVICE` middleware and select **Communication Device Class (Virtual Port Com)**.
6. Generate code while keeping user code.
7. Copy `integration/Core/Inc/MPU6050.h` + `mpu6050_app.h` to `Core/Inc`, and `integration/Core/Src/MPU6050.c` + `mpu6050_app.c` to `Core/Src`. The integration copy adds the missing `<stdio.h>` include seen in the archived warnings.
8. Include `mpu6050_app.h` in `main.c`.
9. After `MX_I2C1_Init()` and `MX_USB_DEVICE_Init()`, call `MPU6050_APP_Init(&hi2c1);`.
10. Inside the infinite loop call `MPU6050_APP_Process();`.
11. Build and flash.
12. Open Windows Device Manager and identify the STMicroelectronics Virtual COM Port.

The reconstructed CDC layer expects CubeMX-generated `usbd_cdc_if.h` and `CDC_Transmit_FS()`.
