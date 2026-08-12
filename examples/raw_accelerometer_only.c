/*
 * Minimal accelerometer-only example matching the debugger screenshot.
 */

#include "mpu6050.h"
#include <stdio.h>

void raw_accelerometer_example(I2C_HandleTypeDef *hi2c)
{
    MPU6050_RawData raw;

    if (MPU6050_Init(hi2c) != MPU6050_STATUS_OK)
    {
        printf("MPU6050 initialization failed\r\n");
        return;
    }

    while (1)
    {
        if (MPU6050_ReadRaw(hi2c, &raw) == MPU6050_STATUS_OK)
        {
            printf(
                "Accel X: %d, Accel Y: %d, Accel Z: %d\r\n",
                raw.accel_x,
                raw.accel_y,
                raw.accel_z
            );
        }
        else
        {
            printf("MPU6050 read failed\r\n");
        }

        HAL_Delay(500U);
    }
}
