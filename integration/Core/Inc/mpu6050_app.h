#ifndef MPU6050_APP_H
#define MPU6050_APP_H
#include "stm32f3xx_hal.h"
void MPU6050_APP_Init(I2C_HandleTypeDef *hi2c);
void MPU6050_APP_Process(void);
#endif
