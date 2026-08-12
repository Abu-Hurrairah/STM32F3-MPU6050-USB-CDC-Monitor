#ifndef APP_MPU6050_H
#define APP_MPU6050_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f3xx_hal.h"

void APP_MPU6050_Init(I2C_HandleTypeDef *hi2c);
void APP_MPU6050_Process(void);

#ifdef __cplusplus
}
#endif

#endif
