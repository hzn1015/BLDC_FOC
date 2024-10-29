#include "app.h"
struct button button1,button2,button3,button4;
static rt_timer_t ButtonTimer=NULL;
unsigned char button1count=0;
unsigned char button2count=0;
unsigned char button3count=0;
unsigned char button4count=0;


unsigned char  read_button1_pin()//按键回调函数
{
  unsigned char flag=0;
	
	flag=HAL_GPIO_ReadPin(GPIOA,ButtonPin1);
	return flag;
}
unsigned char  read_button2_pin()
{

	unsigned char flag=0;
	flag=HAL_GPIO_ReadPin(GPIOC,ButtonPin2);
	
	return flag;
}
unsigned char  read_button3_pin()
{
		unsigned char flag=0;
	flag=HAL_GPIO_ReadPin(GPIOA,ButtonPin3);
	
	return flag;
}

unsigned char  read_button4_pin()
{
		unsigned char flag=0;
	flag=HAL_GPIO_ReadPin(GPIOC,ButtonPin4);
	
	return flag;
}
void button1_callback()
{
	Parkstructure.target_angle=Parkstructure.target_angle+100;
	
}
void button2_callback()
{
		Parkstructure.target_angle=Parkstructure.target_angle-100;
}

void button3_callback()
{
	
}
void button4_callback()//按下4返回主界面
{

}
void ButtonInit()
{
	/**********注册按键，绑定接口*****************/
	button_init(&button1,read_button1_pin , 0);
	button_init(&button2,read_button2_pin , 0);
	button_init(&button3,read_button3_pin , 0);
	button_init(&button4,read_button4_pin , 0);
	/**********按键事件注册，回调函数*************/
	button_attach(&button1,SINGLE_CLICK,button1_callback);
	button_attach(&button2,SINGLE_CLICK,button2_callback);
	button_attach(&button3,SINGLE_CLICK,button3_callback);
	button_attach(&button4,SINGLE_CLICK,button4_callback);
	
	button_start(&button1);
	button_start(&button2);
	button_start(&button3);
	button_start(&button4);

	ButtonTimer=rt_timer_create("button",(void*)button_ticks,0,100,RT_TIMER_FLAG_PERIODIC | RT_TIMER_FLAG_SOFT_TIMER);//创建定时器,用于按键判断
	rt_timer_start(ButtonTimer);//启动定时器

}

