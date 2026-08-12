#include "mpu6050.h"

#define MPU6050_REG_SMPLRT_DIV    0x19U
#define MPU6050_REG_CONFIG        0x1AU
#define MPU6050_REG_GYRO_CONFIG   0x1BU
#define MPU6050_REG_ACCEL_CONFIG  0x1CU
#define MPU6050_REG_ACCEL_XOUT_H  0x3BU
#define MPU6050_REG_PWR_MGMT_1    0x6BU
#define MPU6050_REG_WHO_AM_I      0x75U
#define MPU6050_EXPECTED_WHO_AM_I 0x68U
#define MPU6050_TIMEOUT_MS        100U

static MPU6050_Status write_register(I2C_HandleTypeDef *hi2c, uint8_t reg, uint8_t value)
{
    if (hi2c == NULL)
    {
        return MPU6050_STATUS_INVALID_ARGUMENT;
    }

    if (HAL_I2C_Mem_Write(
            hi2c,
            MPU6050_I2C_ADDRESS,
            reg,
            I2C_MEMADD_SIZE_8BIT,
            &value,
            1U,
            MPU6050_TIMEOUT_MS
        ) != HAL_OK)
    {
        return MPU6050_STATUS_I2C_ERROR;
    }

    return MPU6050_STATUS_OK;
}

static int16_t combine_signed(uint8_t high_byte, uint8_t low_byte)
{
    return (int16_t)(((uint16_t)high_byte << 8U) | (uint16_t)low_byte);
}

MPU6050_Status MPU6050_Init(I2C_HandleTypeDef *hi2c)
{
    uint8_t who_am_i = 0U;
    MPU6050_Status status;

    if (hi2c == NULL)
    {
        return MPU6050_STATUS_INVALID_ARGUMENT;
    }

    if (HAL_I2C_Mem_Read(
            hi2c,
            MPU6050_I2C_ADDRESS,
            MPU6050_REG_WHO_AM_I,
            I2C_MEMADD_SIZE_8BIT,
            &who_am_i,
            1U,
            MPU6050_TIMEOUT_MS
        ) != HAL_OK)
    {
        return MPU6050_STATUS_I2C_ERROR;
    }

    if (who_am_i != MPU6050_EXPECTED_WHO_AM_I)
    {
        return MPU6050_STATUS_NOT_FOUND;
    }

    status = write_register(hi2c, MPU6050_REG_PWR_MGMT_1, 0x00U);
    if (status != MPU6050_STATUS_OK) return status;
    HAL_Delay(100U);

    status = write_register(hi2c, MPU6050_REG_SMPLRT_DIV, 9U);
    if (status != MPU6050_STATUS_OK) return status;

    status = write_register(hi2c, MPU6050_REG_CONFIG, 0x03U);
    if (status != MPU6050_STATUS_OK) return status;

    status = write_register(hi2c, MPU6050_REG_GYRO_CONFIG, 0x00U);
    if (status != MPU6050_STATUS_OK) return status;

    status = write_register(hi2c, MPU6050_REG_ACCEL_CONFIG, 0x00U);
    if (status != MPU6050_STATUS_OK) return status;

    return MPU6050_STATUS_OK;
}

MPU6050_Status MPU6050_ReadRaw(I2C_HandleTypeDef *hi2c, MPU6050_RawData *raw_data)
{
    uint8_t buffer[14];

    if ((hi2c == NULL) || (raw_data == NULL))
    {
        return MPU6050_STATUS_INVALID_ARGUMENT;
    }

    if (HAL_I2C_Mem_Read(
            hi2c,
            MPU6050_I2C_ADDRESS,
            MPU6050_REG_ACCEL_XOUT_H,
            I2C_MEMADD_SIZE_8BIT,
            buffer,
            sizeof(buffer),
            MPU6050_TIMEOUT_MS
        ) != HAL_OK)
    {
        return MPU6050_STATUS_I2C_ERROR;
    }

    raw_data->accel_x = combine_signed(buffer[0], buffer[1]);
    raw_data->accel_y = combine_signed(buffer[2], buffer[3]);
    raw_data->accel_z = combine_signed(buffer[4], buffer[5]);
    raw_data->temperature_raw = combine_signed(buffer[6], buffer[7]);
    raw_data->gyro_x = combine_signed(buffer[8], buffer[9]);
    raw_data->gyro_y = combine_signed(buffer[10], buffer[11]);
    raw_data->gyro_z = combine_signed(buffer[12], buffer[13]);

    return MPU6050_STATUS_OK;
}

void MPU6050_Convert(const MPU6050_RawData *raw_data, MPU6050_Data *converted_data)
{
    if ((raw_data == NULL) || (converted_data == NULL))
    {
        return;
    }

    converted_data->accel_x_g = (float)raw_data->accel_x / MPU6050_ACCEL_LSB_PER_G;
    converted_data->accel_y_g = (float)raw_data->accel_y / MPU6050_ACCEL_LSB_PER_G;
    converted_data->accel_z_g = (float)raw_data->accel_z / MPU6050_ACCEL_LSB_PER_G;

    converted_data->accel_x_ms2 = converted_data->accel_x_g * MPU6050_STANDARD_GRAVITY;
    converted_data->accel_y_ms2 = converted_data->accel_y_g * MPU6050_STANDARD_GRAVITY;
    converted_data->accel_z_ms2 = converted_data->accel_z_g * MPU6050_STANDARD_GRAVITY;

    converted_data->gyro_x_dps = (float)raw_data->gyro_x / MPU6050_GYRO_LSB_PER_DPS;
    converted_data->gyro_y_dps = (float)raw_data->gyro_y / MPU6050_GYRO_LSB_PER_DPS;
    converted_data->gyro_z_dps = (float)raw_data->gyro_z / MPU6050_GYRO_LSB_PER_DPS;

    converted_data->temperature_c =
        ((float)raw_data->temperature_raw / 340.0f) + 36.53f;
}

const char *MPU6050_StatusString(MPU6050_Status status)
{
    switch (status)
    {
        case MPU6050_STATUS_OK: return "OK";
        case MPU6050_STATUS_INVALID_ARGUMENT: return "INVALID_ARGUMENT";
        case MPU6050_STATUS_NOT_FOUND: return "NOT_FOUND";
        case MPU6050_STATUS_I2C_ERROR: return "I2C_ERROR";
        default: return "UNKNOWN";
    }
}
