#ifndef _APP_H_
#define _APP_H_

#include <rtthread.h>
#include "multi_button.h"


#include "main.h"
#include "SVPWM.h"
#include "FOC.h"
#include "as5600.h"
#include "Button.h"


#include "math.h"
#include "stdlib.h"
#include "stdio.h"
//#define HSI_VALUE    (160000000UL)//系统时钟主频，自己定义的，避免cubemx每次编译后主频少了一个0

extern ADC_HandleTypeDef hadc1;
extern ADC_HandleTypeDef hadc2;


extern I2C_HandleTypeDef hi2c1;

extern SPI_HandleTypeDef hspi1;

extern TIM_HandleTypeDef htim1;

extern UART_HandleTypeDef huart1;

extern TIM_HandleTypeDef htim7;

extern DMA_HandleTypeDef hdma_i2c1_tx;
extern DMA_HandleTypeDef hdma_i2c1_rx;
#define PI 3.1415926

int App_main();


#endif

