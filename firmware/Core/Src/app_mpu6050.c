#include "app_mpu6050.h"

#include "mpu6050.h"
#include "usbd_cdc_if.h"

#include <stdio.h>
#include <string.h>

#define APP_OUTPUT_INTERVAL_MS 500U
#define APP_USB_RETRY_COUNT    10U
#define APP_USB_RETRY_DELAY_MS 2U

static I2C_HandleTypeDef *app_i2c = NULL;
static uint32_t last_output_tick = 0U;
static uint8_t sensor_ready = 0U;

static void usb_write(const char *text)
{
    uint32_t attempt;
    uint16_t length;

    if (text == NULL)
    {
        return;
    }

    length = (uint16_t)strlen(text);

    for (attempt = 0U; attempt < APP_USB_RETRY_COUNT; ++attempt)
    {
        if (CDC_Transmit_FS((uint8_t *)text, length) == USBD_OK)
        {
            return;
        }
        HAL_Delay(APP_USB_RETRY_DELAY_MS);
    }
}

void APP_MPU6050_Init(I2C_HandleTypeDef *hi2c)
{
    char line[128];
    MPU6050_Status status;

    app_i2c = hi2c;
    HAL_Delay(1000U);
    usb_write("\r\nSTM32F3 MPU6050 USB CDC Monitor\r\n");

    status = MPU6050_Init(app_i2c);
    if (status != MPU6050_STATUS_OK)
    {
        (void)snprintf(
            line,
            sizeof(line),
            "MPU6050 initialization failed: %s\r\n",
            MPU6050_StatusString(status)
        );
        usb_write(line);
        sensor_ready = 0U;
        return;
    }

    sensor_ready = 1U;
    usb_write("MPU6050 detected at I2C address 0x68\r\n\r\n");
}

void APP_MPU6050_Process(void)
{
    char line[256];
    MPU6050_RawData raw;
    MPU6050_Data data;
    MPU6050_Status status;
    uint32_t now;

    if ((app_i2c == NULL) || (sensor_ready == 0U))
    {
        return;
    }

    now = HAL_GetTick();
    if ((now - last_output_tick) < APP_OUTPUT_INTERVAL_MS)
    {
        return;
    }
    last_output_tick = now;

    status = MPU6050_ReadRaw(app_i2c, &raw);
    if (status != MPU6050_STATUS_OK)
    {
        (void)snprintf(
            line,
            sizeof(line),
            "MPU6050 read failed: %s\r\n",
            MPU6050_StatusString(status)
        );
        usb_write(line);
        return;
    }

    MPU6050_Convert(&raw, &data);

    (void)snprintf(
        line,
        sizeof(line),
        "RAW  AX=%6d AY=%6d AZ=%6d | GX=%6d GY=%6d GZ=%6d\r\n",
        raw.accel_x, raw.accel_y, raw.accel_z,
        raw.gyro_x, raw.gyro_y, raw.gyro_z
    );
    usb_write(line);

    (void)snprintf(
        line,
        sizeof(line),
        "UNIT AX=%+.3fg AY=%+.3fg AZ=%+.3fg | "
        "GX=%+.3fdps GY=%+.3fdps GZ=%+.3fdps | T=%+.2fC\r\n\r\n",
        data.accel_x_g, data.accel_y_g, data.accel_z_g,
        data.gyro_x_dps, data.gyro_y_dps, data.gyro_z_dps,
        data.temperature_c
    );
    usb_write(line);
}
