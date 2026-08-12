#include "mpu6050_app.h"
#include "MPU6050.h"
#include "usbd_cdc_if.h"
#include <stdio.h>
#include <string.h>

static I2C_HandleTypeDef *sensor_i2c;
static uint32_t last_ms;

static void cdc_print(const char *text)
{
    if (text == NULL) return;
    CDC_Transmit_FS((uint8_t *)text, (uint16_t)strlen(text));
}

void MPU6050_APP_Init(I2C_HandleTypeDef *hi2c)
{
    sensor_i2c = hi2c;
    (void)sensor_i2c; /* Original MPU6050 driver uses global hi2c1. */
    HAL_Delay(1000);
    MPU6050_Initialization();
    cdc_print("STM32F3 MPU6050 monitor ready\r\n");
}

void MPU6050_APP_Process(void)
{
    char line[220];
    if (HAL_GetTick() - last_ms < 500U) return;
    last_ms = HAL_GetTick();

    MPU6050_ProcessData(&MPU6050);
    snprintf(line, sizeof(line),
             "RAW AX=%d AY=%d AZ=%d GX=%d GY=%d GZ=%d\r\n",
             MPU6050.acc_x_raw, MPU6050.acc_y_raw, MPU6050.acc_z_raw,
             MPU6050.gyro_x_raw, MPU6050.gyro_y_raw, MPU6050.gyro_z_raw);
    cdc_print(line);

    snprintf(line, sizeof(line),
             "UNIT AX=%.3fg AY=%.3fg AZ=%.3fg GX=%.3fdps GY=%.3fdps GZ=%.3fdps T=%.2fC\r\n",
             MPU6050.acc_x, MPU6050.acc_y, MPU6050.acc_z,
             MPU6050.gyro_x, MPU6050.gyro_y, MPU6050.gyro_z,
             MPU6050.temperature);
    cdc_print(line);
}
