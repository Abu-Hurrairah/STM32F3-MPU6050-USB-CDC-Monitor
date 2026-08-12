/* Add to CubeMX-generated main.c */
#include "mpu6050_app.h"

/* after peripheral initialization */
MPU6050_APP_Init(&hi2c1);

/* inside while(1) */
MPU6050_APP_Process();
HAL_Delay(1);
