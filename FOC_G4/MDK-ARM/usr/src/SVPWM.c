#include "app.h"

_svpwm SVPWMStructure;
//判断扇区
//输入参数：静止坐标系电流
unsigned char JudgeSection(_svpwm* SVPWM)
{
	float U_ref1=0,U_ref2=0,U_ref3=0;
	unsigned char A=0,B=0,C=0,N=0;
	//计算中间变量
	U_ref1=SVPWM->U.U_beta;
	U_ref2=(__sqrtf(3)*(SVPWM->U.U_alpha)-SVPWM->U.U_beta)/2;
	U_ref3=(-__sqrtf(3)*(SVPWM->U.U_alpha)-SVPWM->U.U_beta)/2;
	
	if(U_ref1>0)
		A=1;
	else 
		A=0;
	
	if(U_ref2>0)
		B=1;
	else 
		B=0;
	
	if(U_ref3>0)
		C=1;
	else 
		C=0;
	
	N=4*C+2*B+A;
	
	return N;
}

//计算作用时间
//输入Alpha-beta作用量，扇区，PWM周期，直流母线电压，输出T1，T2
void CalculateActionTime(_svpwm* SVPWM,unsigned char N)
{
	float X=0,Y=0,Z=0;//中间变量
	float Ts=0,T1=0,T2=0;
	float Udc=0,U_beta=0,U_alpha=0;
	
	Ts=SVPWM->Ts;
	Udc=SVPWM->Udc;
	U_beta=SVPWM->U.U_beta;
	U_alpha=SVPWM->U.U_alpha;
	
	//计算中间变量
//	X=(sqrt(3)*Ts*(U_beta))/(Udc);
//	Y=(sqrt(3)*Ts/(2*Udc))*(sqrtf(3)*(U_alpha)+U_beta);
//	Z=(sqrt(3)*Ts/(2*Udc))*(-sqrtf(3)*(U_alpha)+U_beta);
	
	switch(N)
	{
		case 1:{Y=(__sqrtf(3)*Ts/(2*Udc))*(__sqrtf(3)*(U_alpha)+U_beta); Z=(__sqrtf(3)*Ts/(2*Udc))*(-__sqrtf(3)*(U_alpha)+U_beta); T1=Z;T2=Y;};break;
		case 2:{Y=(__sqrtf(3)*Ts/(2*Udc))*(__sqrtf(3)*(U_alpha)+U_beta); X=(__sqrtf(3)*Ts*(U_beta))/(Udc);                         T1=Y;T2=-X;};break;
		case 3:{Z=(__sqrtf(3)*Ts/(2*Udc))*(-__sqrtf(3)*(U_alpha)+U_beta);X=(__sqrtf(3)*Ts*(U_beta))/(Udc);                         T1=-Z;T2=X;};break;
		case 4:{X=(__sqrtf(3)*Ts*(U_beta))/(Udc);Z=(__sqrtf(3)*Ts/(2*Udc))*(-__sqrtf(3)*(U_alpha)+U_beta);                         T1=-X;T2=Z;};break;
		case 5:{X=(__sqrtf(3)*Ts*(U_beta))/(Udc);Y=(__sqrtf(3)*Ts/(2*Udc))*(__sqrtf(3)*(U_alpha)+U_beta);                          T1=X;T2=-Y;};break;
		case 6:{Y=(__sqrtf(3)*Ts/(2*Udc))*(__sqrtf(3)*(U_alpha)+U_beta);	Z=(__sqrtf(3)*Ts/(2*Udc))*(-__sqrtf(3)*(U_alpha)+U_beta);T1=-Y;T2=-Z;};break;
	}
	
	if(((T1)+(T2))>Ts)//过调制处理
	{
		T1=(T1)/((T1)+(T2))*Ts;
		T2=(T2)/((T1)+(T2))*Ts;
	}
	
	SVPWM->T_first=T1;
	SVPWM->T_second=T2;

}

//计算矢量切换点
void CalculateVector(_svpwm* SVPWM,unsigned char N)
{
//	float Ta=0,Tb=0,Tc=0;
	float T[3]={0};
	float Tcm1=0,Tcm2=0,Tcm3=0;
	T[0]=(SVPWM->Ts-SVPWM->T_first-SVPWM->T_second)/4;
	T[1]=T[0]+SVPWM->T_first/2;
	T[2]=T[1]+SVPWM->T_second/2;
	

	switch(N)
	{
		case 1:{Tcm1=T[1];Tcm2=T[0],Tcm3=T[2];};break;
		case 2:{Tcm1=T[0];Tcm2=T[2],Tcm3=T[1];};break;
		case 3:{Tcm1=T[0];Tcm2=T[1],Tcm3=T[2];};break;
		case 4:{Tcm1=T[2];Tcm2=T[1],Tcm3=T[0];};break;
		case 5:{Tcm1=T[2];Tcm2=T[0],Tcm3=T[1];};break;
		case 6:{Tcm1=T[1];Tcm2=T[2],Tcm3=T[0];};break;
	
	}
	
	SVPWM->T_cm1=Tcm1;
	SVPWM->T_cm2=Tcm2;
	SVPWM->T_cm3=Tcm3;
	
	//找到最小值
	
	
	

}

int CalculatePWMValue(float T)
{
	
	int value=0;
	value=(T*2/(T_s))*TIM_Automatic_reload_value;
	if(value>=(TIM_Automatic_reload_value-80))
		value=TIM_Automatic_reload_value-80;
	return value;

}

//SVPWM结构体初始化
void SVPWMstructInit(_svpwm *SVPWM)
{
		SVPWM->U.U_alpha=0;
		SVPWM->U.U_beta=0;
		SVPWM->Ts=T_s;
		SVPWM->Udc=U_dc;

}
//产生SVPWM
//输入Alpha-Beta电压，输出三相逆变电压
void Set_SVPWM(struct Alpha_Beta* U)
{
	unsigned char N=0;
	int PWM1=0,PWM2=0,PWM3=0,PWM4=0;
	// 4us
	SVPWMStructure.U.U_alpha=U->U_alpha;
	SVPWMStructure.U.U_beta=U->U_beta;

	
	N=JudgeSection(&SVPWMStructure);//判断扇区  5.4us
	
	CalculateActionTime(&SVPWMStructure,N);//计算作用时间   5.96us
	
	CalculateVector(&SVPWMStructure,N);//计算扇区矢量切换点  4.82us
	
	PWM1=CalculatePWMValue(SVPWMStructure.T_cm1);
	PWM2=CalculatePWMValue(SVPWMStructure.T_cm2);
	PWM3=CalculatePWMValue(SVPWMStructure.T_cm3);    //  5.78us
	//调试用的，表示能够生产马鞍波
	//rt_kprintf("%d,%d,%d\n",(int)(SVPWM->T_cm1*100000000),(int)(SVPWM->T_cm2*100000000),(int)(SVPWM->T_cm3*100000000));
	Set_A_PWM(PWM1);
	Set_B_PWM(PWM2);    //0.02us
	Set_C_PWM(PWM3);
	
	//rt_kprintf("%d,%d,%d\n",PWM1,PWM2,PWM3);
	
}