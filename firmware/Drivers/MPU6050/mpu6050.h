#ifndef MPU6050_H
#define MPU6050_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f3xx_hal.h"
#include <stdint.h>

#define MPU6050_I2C_ADDRESS_7BIT  0x68U
#define MPU6050_I2C_ADDRESS       (MPU6050_I2C_ADDRESS_7BIT << 1U)

#define MPU6050_ACCEL_LSB_PER_G    16384.0f
#define MPU6050_GYRO_LSB_PER_DPS   131.0f
#define MPU6050_STANDARD_GRAVITY   9.80665f

typedef enum
{
    MPU6050_STATUS_OK = 0,
    MPU6050_STATUS_INVALID_ARGUMENT,
    MPU6050_STATUS_NOT_FOUND,
    MPU6050_STATUS_I2C_ERROR
} MPU6050_Status;

typedef struct
{
    int16_t accel_x;
    int16_t accel_y;
    int16_t accel_z;
    int16_t temperature_raw;
    int16_t gyro_x;
    int16_t gyro_y;
    int16_t gyro_z;
} MPU6050_RawData;

typedef struct
{
    float accel_x_g;
    float accel_y_g;
    float accel_z_g;
    float accel_x_ms2;
    float accel_y_ms2;
    float accel_z_ms2;
    float gyro_x_dps;
    float gyro_y_dps;
    float gyro_z_dps;
    float temperature_c;
} MPU6050_Data;

MPU6050_Status MPU6050_Init(I2C_HandleTypeDef *hi2c);
MPU6050_Status MPU6050_ReadRaw(I2C_HandleTypeDef *hi2c, MPU6050_RawData *raw_data);
void MPU6050_Convert(const MPU6050_RawData *raw_data, MPU6050_Data *converted_data);
const char *MPU6050_StatusString(MPU6050_Status status);

#ifdef __cplusplus
}
#endif

#endif
