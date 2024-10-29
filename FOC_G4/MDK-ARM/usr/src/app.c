#include "app.h"


rt_thread_t FOC_Electricy_PID=RT_NULL;//电流环PID
rt_thread_t FOC_Speed_PID=RT_NULL;//速度环PID
rt_thread_t FOC_Position_PID=RT_NULL;//位置环PID
rt_thread_t UART_Printf_t=RT_NULL;//串口打印线程
rt_thread_t FOC_Angle_Calibration_rt=RT_NULL;//角度校准线程

rt_timer_t Timer_t=RT_NULL;
rt_mq_t thread_mq_t=RT_NULL;

rt_sem_t adc_sem=RT_NULL;//adc信号量
rt_sem_t Speed_PID_sem=RT_NULL;//速度环PID信号量
rt_sem_t Position_PID_sem=RT_NULL;//位置环PID信号量
float speed=5;

void Electricy_PID_Callback()
{
	static float Last_Angle=0,Last_Speed=0;
	unsigned char speed_count=0,position_count=0;//计数
	while(1)
	{
		rt_sem_take(adc_sem, RT_WAITING_FOREVER);//通知线程可以进行电流环
		GainElectricity(&Clarkstructure);//获取电机电流
		HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_10);
   	FOC_Calculate();
		speed_count++;
		position_count++;
		if(speed_count==5)//每5个电流环处理一次速度环
		{
			speed_count=0;
			rt_sem_release(Speed_PID_sem);//释放速度环PID信号量，切换到速度环
		}
		if(position_count==10)
		{
						position_count=0;
			rt_sem_release(Position_PID_sem);//释放速度环PID信号量，切换到速度环
		}

		
	}

}

void Speed_PID_Callback()
{
	
	char data[128];
	char count=0;
	while(1)
	{
		
		rt_sem_take(Speed_PID_sem, RT_WAITING_FOREVER);//通知线程可以进行速度环
	

		count++;
		if(count==5)
		{
			count=0;
			Parkstructure.Theat_speed=CalculateSpeed(Parkstructure.Angle_r,0,0.005);	
			Parkstructure.I_q=Speed_PID(Parkstructure.target_speed,Parkstructure.Theat_speed);
			
	
		}


		
	}


}

void Position_PID_Callback()
{
		
	while(1)
	{
		
		rt_sem_take(Position_PID_sem, RT_WAITING_FOREVER);//通知线程可以进行速度环
	//	Parkstructure.target_angle=Parkstructure.target_angle+10;
			//HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_10);
		Parkstructure.target_speed=Position_PID(Parkstructure.target_angle,Parkstructure.Angle_r);
	
	}

}
//打印线程
void uart_print()
{
	char data[128];
	while(1)
	{
		
		
	//	sprintf(data,"%d,%d,%d,%d,%d\n",(int)Parkstructure.Theat_speed,(int)(Parkstructure.ActI_q*1000),(int)(Parkstructure.ActI_d*1000),(int)Parkstructure.target_speed,(int)(Parkstructure.Angle_r));
		//sprintf(data,"%d,%d,%d,%d,%d,%d,%d,%d\n",(int)(Clarkstructure.I.U_alpha*1000),(int)(Clarkstructure.I.U_beta*1000),(int)(Parkstructure.I.U_alpha*10000),(int)(Parkstructure.I.U_beta*10000),(int)(Parkstructure.ActI_d*10000),\
			(int)(Parkstructure.ActI_q*10000),(int)(Parkstructure.thread),(int)(Parkstructure.E_Angle*100));
			sprintf(data,"%d,%d,%d,%d,%d,%d,%d,%ld,%d\n",(int)(Clarkstructure.I_a*10000),(int)(Clarkstructure.I_b*10000),(int)(Clarkstructure.I_c*10000),(int)(Parkstructure.ActI_d*10000),\
			(int)(Parkstructure.ActI_q*10000),(int)(Parkstructure.I_q*10000),(int)(Parkstructure.Theat_speed),(long int)(Parkstructure.Angle_r),(int)(800));
				
		//	sprintf(data,"%d,%d,%d\n",(int)(Parkstructure.Angle_r*P_n*100),(int)(Parkstructure.E_Angle*100),(int)(Parkstructure.thread));
		//	sprintf(data,"%d\n",Angle_Dis);
		HAL_UART_Transmit_DMA(&huart1,data,rt_strlen(data));
		//HAL_UART_Transmit_DMA(&huart1,data,rt_strlen(data));
		//	HAL_UART_Transmit_DMA(&huart1,data,rt_strlen(data));
		rt_thread_mdelay(20);
	}



}

void FOC_Calibation_Callback()
{

	long int angle_e=0,_angle_e=0,angle_r=0,last_angle_e=0,last_angle_r=0,last_angle_init=0,count=0;//电角度，机械角度，计数值
	char data[128];
	int angle_err=0,angle_r_count=0,err_sum=0;//角度误差
	float I_d=0.5,I_q=0,_Cos=0,_Sin=0;
//	while(1)
//	{
	
		struct Alpha_Beta * U;
		U=(struct Alpha_Beta *)malloc(sizeof(struct Alpha_Beta));

		angle_e=(PI/3)*(count);//用于循环更新电角度，从0开始到2*PI
		
		_Cos=cosf(angle_e);
		_Sin=sinf(angle_e);
	
	
		U->U_alpha=I_d*_Cos-I_q*_Sin;
		U->U_beta= I_d*_Sin+I_q*_Cos;
		
	
		Set_SVPWM(U);
		
		rt_thread_mdelay(2000);

		int angle=getRawAngle();//获取机械角度编码

		
		Angle_Dis=angle;
		

		free(U);


}


//按键初始化，绑定按键

void Button_Init()
{


}
//用户主函数，重新封装避免因为cubemx导致代码重构
int App_main()
{
	
	//创建FOC控制线程
	
//	ADC1->CFGR=(ADC1->CFGR|(1<<11));
	GainCurrentInitADC();
	Parkstructure.I_q=0.2;
	PWM_Start();
	ButtonInit();
	FOC_Electricy_PID=rt_thread_create("FOC",Electricy_PID_Callback,0,254,0,1);
	FOC_Speed_PID=rt_thread_create("FOC",Speed_PID_Callback,0,512,2,1);
	FOC_Position_PID=rt_thread_create("FOC",Position_PID_Callback,0,512,3,1);
	FOC_Angle_Calibration_rt=rt_thread_create("FOC",FOC_Calibation_Callback,0,512,0,1);
	
	
	UART_Printf_t=rt_thread_create("uart",uart_print,0,512,4,100);
	
	adc_sem=rt_sem_create("adc",0,RT_IPC_FLAG_FIFO);
	Speed_PID_sem=rt_sem_create("speed",0,RT_IPC_FLAG_FIFO);
	Position_PID_sem=rt_sem_create("position",0,RT_IPC_FLAG_FIFO);
	SVPWMstructInit(&SVPWMStructure);
//	rt_timer_start(Timer_t);
	rt_thread_startup(FOC_Electricy_PID);
	rt_thread_startup(UART_Printf_t);//启动串口
	rt_thread_startup(FOC_Speed_PID);
  rt_thread_startup(FOC_Position_PID);
	
//	rt_thread_startup(FOC_Angle_Calibration_rt);
	
	return 0;
}

/**
  * @brief  Conversion complete callback in non-blocking mode.
  * @param hadc ADC handle
  * @retval None
*/
void HAL_ADCEx_InjectedConvCpltCallback(ADC_HandleTypeDef *hadc)
{
	static int count=0;
	if(hadc->Instance==ADC1)
	{		
	}
  else if(hadc->Instance==ADC2)
	{
		count++;
		if(count==8)
		{
			count=0;
			rt_sem_release(adc_sem);
		}
	//	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_10,GPIO_PIN_RESET);

//		
	}



}
