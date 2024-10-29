#include "app.h"
_clark Clarkstructure;//克拉克变换结构体
_park Parkstructure;//帕克变换结构体
_park AntiParkstructure;//帕克逆变换结构体

int ADC_A_Mid=2040;
int ADC_B_Mid=2080;
int ADC_C_Mid=2100;

int Angle_Dis=0; //角度偏差
//Clark变换
void ClarkConversion(_clark* clark)
{
	float Ia=0,Ib=0,Ic=0;
	Ia=clark->I_a;
	Ib=clark->I_b;
	Ic=clark->I_c;
	
	clark->I.U_alpha=(Ia-0.5*(Ib+Ic))*2/3;
	clark->I.U_beta=__sqrtf(3)/3*(Ib-Ic);

}


//Park变换
void ParkConversion(_park* park,_clark *Clark)
{
	
//	park->U_d=(park->U->U_alpha)*cosf(park->thread)+(park->U->U_beta)*sinf(park->thread);
//	park->U_q=-(park->U->U_alpha)*sinf(park->thread)+(park->U->U_beta)*cosf(park->thread);

	park->I_d=(Clark->I.U_alpha)*cosf(park->E_Angle)+(Clark->I.U_beta)*sinf(park->E_Angle);
	park->I_q=-(Clark->I.U_alpha)*sinf(park->E_Angle)+(Clark->I.U_beta)*cosf(park->E_Angle);
	
	
}

//Park逆变换
void AntiParkConversion(_park* park)
{
	
	float value1=0,value2=0;
	value1=cosf(park->E_Angle);
	value2=sinf(park->E_Angle);
	(park->I.U_alpha)=(park->I_d*value1-park->I_q*value2);
	(park->I.U_beta)=(park->I_d*value2+park->I_q*value1);
	
}

//计算角度
void CalculateAngle(float angle_speed)
{
	static float angle=0,last_angle=0;
	static int count=0,flag=0;//用于记录圈数
	angle=4096-getRawAngle();
	if(flag==0)
	{
		flag++;
		last_angle=angle;
		
	}
	if(angle-last_angle<-1024)//表示过0点重置一圈
	{
		count++;
	}
	else if(angle-last_angle>1024)
	{
		count--;
	}
	last_angle=angle;
//	angle=angle+2*PI*angle_speed*0.0002;
//	if(angle>2*PI)
//		angle=angle-2*PI;
//	if(angle<-2*PI)
//		angle=angle+2*PI;
	Parkstructure.thread=(angle);
//	Parkstructure.Angle_r=(4096-getRawAngle());
	Parkstructure.Angle_r=angle*360/4096+count*360;//计算角度
	Parkstructure.E_Angle=GainElectricajAngle(angle,P_n);
}
float k=0.1;
float CalculateSpeed(float angle,float target,float T)
{
	static float last_angle=0,last_speed=0,flag=0;
	
	float speed=0;
	float value=0;
	if(flag==0)
	{
		flag=1;
			last_angle=angle;
	}
	speed=((angle-last_angle));//计算角度
	last_angle=angle;
	
	last_speed=Filter(k,speed,last_speed);
	return last_speed/T;
	
}

float GainElectricajAngle(float Angle,int Pn)
{
	float angle=0;
	
	float angle_offset=0;
//	
	if(Angle>0.67&&Angle<1.58)
		angle_offset=4.74;	
	else if(Angle>=1.58&&Angle<2.49)
		angle_offset=4.77;
	else if(Angle>=2.49&&Angle<3.39)
		angle_offset=4.90;
	else if(Angle>=3.39&&Angle<4.28)
		angle_offset=4.92;
	else if(Angle>=4.28&&Angle<5.17)
		angle_offset=4.82;
	else if(Angle>=5.17&&Angle<6.06)
		angle_offset=4.77;
		else if(Angle<2.49||Angle>=6.06)
		angle_offset=4.77;
	
	angle=Angle*Pn*2*PI/4096.0-angle_offset;
	//angle=((Angle)*2*PI/4096)*P_n;
//		if(angle>=2*PI)
//			angle=angle-2*PI;
//		if(angle<=-2*PI)
//			angle=angle+2*PI;
	while(angle>2*PI)
	{
		angle=angle-2*PI;
	}
	while(angle<-2*PI)
	{
		angle=angle+2*PI;
	}
	return angle;

}
//VF强拖
//输入转速(n/s)
float VF_Control(float Thead)
{
	float U_1=0;
	float K_n=50;
	float Psi=0.0005;
	U_1=4.44*Thead*K_n*Psi;
	
	Parkstructure.I_q=0.8;
	Parkstructure.I_d=0;
	AntiParkConversion(&Parkstructure);
//	Parkstructure.I.U_alpha=U_1*cosf(Parkstructure.E_Angle);
//	Parkstructure.I.U_beta=U_1*cosf(Parkstructure.E_Angle+PI/2);
//	
		
//		float U_a=Parkstructure.I.U_alpha;
//		float U_b=-0.5*Parkstructure.I.U_alpha+sqrtf(3)/2*Parkstructure.I.U_beta;
//		float U_c=-0.5*Parkstructure.I.U_alpha-sqrtf(3)/2*Parkstructure.I.U_beta;
//		
//		
//	
//		Set_A_PWM(U_a*667);
//		Set_B_PWM(U_b*667);    //0.02us
//		Set_C_PWM(U_c*667);
	
	return U_1;
}
//初始化获取电流adc
void GainCurrentInitADC()
{
	HAL_ADCEx_Calibration_Start(&hadc1,ADC_SINGLE_ENDED);
	HAL_ADCEx_Calibration_Start(&hadc2,ADC_SINGLE_ENDED);
	
	HAL_ADCEx_InjectedStart(&hadc1);
	HAL_ADCEx_InjectedStart(&hadc2);
	
	__HAL_ADC_ENABLE_IT(&hadc1, ADC_IT_JEOC);
	__HAL_ADC_ENABLE_IT(&hadc2, ADC_IT_JEOC);

}


float ADC_turn_electricity(float K,int ADC_Value,int mid_Value)
{
	float Electricity=0;
	Electricity=K*(ADC_Value-mid_Value);
	
	return Electricity;
}

//电流ADC校正
void ElectricityRectify()
{
	
	

		

	
}

//获取电流

void GainElectricity(_clark *Clark)
{
	static int cout=0;
	int A_value=0,B_value=0,C_value=0;;
//	if(cout<50)
//	{
//		A_value=A_value+HAL_ADCEx_InjectedGetValue(&hadc2,ADC_INJECTED_RANK_1);
//		B_value=B_value+HAL_ADCEx_InjectedGetValue(&hadc1,ADC_INJECTED_RANK_2);
//		C_value=C_value+HAL_ADCEx_InjectedGetValue(&hadc1,ADC_INJECTED_RANK_1);
//		cout++;
//	}
//	else if(cout==50)//进行数据校正
//	{
//		A_value=A_value/50;
//		B_value=B_value/50;
//		C_value=C_value/50;
//	
//		ADC_A_Mid=A_value;
//		ADC_B_Mid=B_value;
//		ADC_C_Mid=C_value;
//		cout++;
//	//
//	}
//	else
//	{
		B_value=HAL_ADCEx_InjectedGetValue(&hadc1,ADC_INJECTED_RANK_2);
		C_value=HAL_ADCEx_InjectedGetValue(&hadc1,ADC_INJECTED_RANK_1);
		A_value=HAL_ADCEx_InjectedGetValue(&hadc2,ADC_INJECTED_RANK_1);
		
		Clark->I_a=ADC_turn_electricity(K_a,A_value,ADC_A_Mid);
		Clark->I_b=ADC_turn_electricity(K_b,B_value,ADC_B_Mid);
		Clark->I_c=ADC_turn_electricity(K_c,C_value,ADC_C_Mid);
//		Clark->I_a=A_value;
//		Clark->I_b=B_value;
//		Clark->I_c=C_value;
	///Clark->I_c=-Clark->I_b-Clark->I_a;
//	}

		

}
//I_Q_PI电流环
//参数当前值和参考值
float Q_Kp=0.500000006,Q_KI=0.00059999997;
float I_Q_Limit=10000;
float I_Q_PI_Electricity(float Target,float Act_Value)
{
	static float PID_Value=0,PID_Sum=0;
	float err=0;
	err=(Act_Value-Target);
		PID_Sum=PID_Sum+err;
	//积分限幅
	if(PID_Sum>I_Q_Limit)
		PID_Sum=I_Q_Limit;
	if(PID_Sum<-I_Q_Limit)
		PID_Sum=-I_Q_Limit;
	PID_Value=Q_Kp*err+Q_KI*PID_Sum;
	
	
	return PID_Value;
	
	

}

//I_Q_PI电流环
//参数当前值和参考值
float D_Kp=1.5,D_KI=0.00059999998;
float I_D_Limit=2000;
float I_D_PI_Electricity(float Target,float Act_Value)
{
	static float PID_Value=0,PID_Sum=0;
	float err=0;
	err=(Act_Value-Target);
		PID_Sum=PID_Sum+err;
	//积分限幅
	if(PID_Sum>I_D_Limit)
		PID_Sum=I_D_Limit;
	if(PID_Sum<-I_D_Limit)
		PID_Sum=-I_D_Limit;
	PID_Value=D_Kp*err+D_KI*PID_Sum;
	
	return PID_Value;
	

}

//float Speed_Kp=0.1,Speed_Ki=0;
float Speed_Kp=0.005,Speed_Ki=0.0001;
float PID_Value=0,PID_Sum=0;
float speed_limit=5000;
float Speed_PID(float Target,float Act_Value)
{
	//static float PID_Value=0,PID_Sum=0;
	float err=0;
	err=(Target-Act_Value);
	
//	if(abs(err)<200)
			PID_Sum+=err;
	//积分限幅
	if(PID_Sum>speed_limit)
		PID_Sum=speed_limit;
	if(PID_Sum<-speed_limit)
		PID_Sum=-speed_limit;
	PID_Value=Speed_Kp*err+Speed_Ki*PID_Sum;
	
	if(PID_Value>4)//输出限幅
		PID_Value=4;
	
	if(PID_Value<-4)
		PID_Value=-4;
	return PID_Value;


}

float Position_Kp=8,Position_Ki=0.00200000024,Position_Kd;
float Position_PID_Value=0,Position_PID_Sum=0;
float Position_limit=100000;
float Position_PID(float Target,float Act_Value)
{
	//static float PID_Value=0,PID_Sum=0;
	static float last_err=0;
	float err=0,D=0;
	err=(Target-Act_Value);
	if(abs(err)<2)
	{
		err=0;
	}
//	if(abs(err)<200)
			Position_PID_Sum+=err;
	//积分限幅
	if(Position_PID_Sum>Position_limit)
		Position_PID_Sum=Position_limit;
	if(Position_PID_Sum<-Position_limit)
		Position_PID_Sum=-Position_limit;
	Position_PID_Value=Position_Kp*err+Position_Ki*Position_PID_Sum+Position_Kd*(err-last_err);
	last_err=err;
	if(Position_PID_Value>1000)
		Position_PID_Value=1000;
	if(Position_PID_Value<-1000)
		Position_PID_Value=-1000;
	
	return Position_PID_Value;


}
float Filter(float K,float Value,float Last_Value)
{
		return K*Value+(1-K)*Last_Value;
}

#if defined(CCMARM)

#elif defined (__CC_ARM)|| )

#endif
//把foc代码放到CCM SRAM内存中，运行速度更快
__attribute__((section(".ccmram")))
//FOC计算

float K_Q=0.600000024,K_D=0.100000001;
float L=0.000172;
void FOC_Calculate() 
{
	static float Last_I_q=0,Last_I_d=0;
		__disable_irq();

	float Cos_value=0,Sin_value=0;

//	VF_Control(3);
	
	CalculateAngle(3);
	Cos_value=cosf(Parkstructure.E_Angle);
	Sin_value=sinf(Parkstructure.E_Angle);
	
	ClarkConversion(&Clarkstructure);//Clark变换
	
	//帕克变换
	Parkstructure.ActI_d=(Clarkstructure.I.U_alpha)*Cos_value+(Clarkstructure.I.U_beta)*Sin_value;
	Parkstructure.ActI_q=-(Clarkstructure.I.U_alpha)*Sin_value+(Clarkstructure.I.U_beta)*Cos_value;
	
//	
	Parkstructure.ActI_q=Filter(K_Q,Parkstructure.ActI_q,Last_I_q);
	Parkstructure.ActI_d=Filter(K_D,Parkstructure.ActI_d,Last_I_d);
	Last_I_q=Parkstructure.ActI_q;
	Last_I_d=Parkstructure.ActI_d;
	//Park逆变换
	float U_q=0,U_d=0;
	U_q=I_Q_PI_Electricity(Parkstructure.I_q,Last_I_q);//Parkstructure.Theat_speed*(L);
	U_d=I_D_PI_Electricity(Parkstructure.I_d,Last_I_d);//Parkstructure.Theat_speed*L*Last_I_q;
	(Parkstructure.I.U_alpha)=(U_d*Cos_value-U_q*Sin_value);
	(Parkstructure.I.U_beta)=(U_d*Sin_value+U_q*Cos_value);


	//ParkConversion(&Parkstructure,&Clarkstructure);//park变换
		
		
		Set_SVPWM(&(Parkstructure.I));

		__enable_irq();

}

//电机角度校准
void FOC_Angle_Calibration()
{
	static long int angle_e=0,angle_r=0,last_angle_r=0,count=0;//电角度，机械角度，计数值
	static int angle_err=0;//角度误差
	float I_d=1,I_q=0,_Cos=0,_Sin=0;
	
	struct Alpha_Beta * U;
	
	U=(struct Alpha_Beta *)malloc(sizeof(struct Alpha_Beta));

		angle_e=(PI/3)*(count);//用于循环更新电角度，从0开始到2*PI
		_Cos=cosf(angle_e);
		_Sin=sinf(angle_e);
	
		U->U_alpha=I_d*_Cos-I_q*_Sin;
		U->U_beta= I_d*_Sin+I_q*_Cos;
		
		Set_SVPWM(U);
		
		angle_r=getRawAngle();//获取当前机械角度
		angle_err=angle_r-angle_e*2048/PI;
		
		

//	for(int i=0;i<6;i++)
//  {
	
	Parkstructure.Angle_r=getRawAngle();//获取当前机械角度
	
	free(U);
	//}
	


}