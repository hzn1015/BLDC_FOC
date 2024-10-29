#ifndef __FOC_H_
#define __FOC_H_

#include "app.h"

#define P_n 7 //电机极对数
#define R_s 0.01 //采样电阻zuzhi4

#define K_a 0.00201416
#define K_b 0.00201416
#define K_c 0.00201416


#define Angle_Turn_Rad(ang)   (ang*0.0174532f)
#define Rad_Turn_Angle(ang)   (ang*57.29578f)
typedef struct 
{
	
	float I_a;
	float I_b;
	float I_c;
	struct Alpha_Beta I;
}_clark;

typedef struct 
{
	
	struct Alpha_Beta I;
	
	float I_d;//参考电流
	float I_q;
	
	float ActI_d;//实际电流
	float ActI_q;
	
	struct Alpha_Beta U;
	
	float U_d;//参考电压
	float U_q;
		
	float thread;//角度adc值
	float Angle_r;//机械角度
	float E_Angle;//电角度
	float target_angle;
	
	float Theat_speed;
	float target_speed;
	
	
}_park;

extern _clark Clarkstructure;//克拉克变换结构体
extern _park Parkstructure;//帕克变换结构体
extern _park AntiParkstructure;//帕克逆变换结构体

extern int Angle_Dis;
void ClarkConversion(_clark* clark);
void ParkConversion(_park* park,_clark *Clark);
void AntiParkConversion(_park* park);

void CalculateAngle(float angle_speed);
float CalculateSpeed(float angle,float target,float T);
float VF_Control(float Thead);
void GainCurrentInitADC();
float ADC_turn_electricity(float K,int ADC_Value,int mid_Value);
void GainElectricity(_clark *Clark);
void FOC_Calculate();
float GainElectricajAngle(float Angle,int Pn);
float Filter(float K,float Value,float Last_Value);
void ElectricityRectify();
float Speed_PID(float Target,float Act_Value);//速度环
float Position_PID(float Target,float Act_Value);

void FOC_Angle_Calibration();
#endif