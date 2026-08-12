/*
 * Copy the marked sections into the CubeMX-generated Core/Src/main.c.
 */

/* USER CODE BEGIN Includes */
#include "app_mpu6050.h"
/* USER CODE END Includes */

int main(void)
{
    HAL_Init();
    SystemClock_Config();

    MX_GPIO_Init();
    MX_I2C1_Init();
    MX_USB_DEVICE_Init();

    /* USER CODE BEGIN 2 */
    APP_MPU6050_Init(&hi2c1);
    /* USER CODE END 2 */

    while (1)
    {
        /* USER CODE BEGIN WHILE */
        APP_MPU6050_Process();
        HAL_Delay(1U);
        /* USER CODE END WHILE */
    }
}
