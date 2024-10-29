#ifndef __SVPWM_H__
#define __SVPWM_H__

#include "app.h"
//重新映射端口

#define PWM_Start()    {	HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_1);\
	HAL_TIMEx_PWMN_Start(&htim1,TIM_CHANNEL_1);\
	HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_2);\
	HAL_TIMEx_PWMN_Start(&htim1,TIM_CHANNEL_2);\
	HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_3);\
	HAL_TIMEx_PWMN_Start(&htim1,TIM_CHANNEL_3);\
	HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_4);\
  __HAL_TIM_ENABLE_IT(&htim1, TIM_IT_UPDATE);\
}

#define Set_A_PWM(Value) __HAL_TIM_SetCompare(&htim1,TIM_CHANNEL_1,Value); 
#define Set_B_PWM(Value) __HAL_TIM_SetCompare(&htim1,TIM_CHANNEL_2,Value); 
#define Set_C_PWM(Value) __HAL_TIM_SetCompare(&htim1,TIM_CHANNEL_3,Value); 
#define Set_D_PWM(Value) __HAL_TIM_SetCompare(&htim1,TIM_CHANNEL_4,Value); 

#define T_s 0.0000025 //PWM周期
#define U_dc 12//直流母线电压
#define TIM_Automatic_reload_value 2000-1//定时器自动重装值
	

#define Sqrt_3 1.7320508
struct Alpha_Beta
{
	float U_alpha;
	float U_beta;
};
typedef struct 
{
	struct Alpha_Beta U;
	float Ts;
	float Udc;
	
	float T_first;
	float T_second;
	
	float T_cm1;
	float T_cm2;
	float T_cm3;
	float T_cm4;//ADC采样触发点

}_svpwm;

extern _svpwm SVPWMStructure;
unsigned char JudgeSection(_svpwm * SVPWM);
int CalculatePWMValue(float T);
void Set_SVPWM(struct Alpha_Beta* U);
void SVPWMstructInit(_svpwm *SVPWM);

#endif
