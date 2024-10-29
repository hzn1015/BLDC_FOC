#include "as5600.h"
extern  UART_HandleTypeDef huart1;


void test_as5600()
{
uint8_t a1=0x00;
uint8_t a2=0x55;	
	for(int i=0;i<255;i++)
	{
	if(HAL_I2C_Master_Transmit(&hi2c1,a1,&a2,1,100)==HAL_OK)
	{	char data[128];
		sprintf(data,"AT 0x%x have a mod\r\n",a1);
		HAL_UART_Transmit(&huart1,data,rt_strlen(data),100);
	}
	a1++;
	}
}

uint8_t readOneByte(uint8_t in_adr)
{
uint8_t a3=0;
HAL_I2C_Master_Transmit(&hi2c1,0x6c,&in_adr,1,10);
		HAL_I2C_Master_Receive(&hi2c1,0x6c,&a3,1,10);


return a3;
}

uint16_t readtwoByte(uint8_t h_adr , uint8_t l_adr)
{
uint8_t high=0;
uint8_t low=0;	
uint16_t retVal = 0;
	
high = readOneByte(h_adr);
	//HAL_I2C_Mem_Read(&hi2c1,0x6c,h_adr,I2C_MEMADD_SIZE_8BIT,&high,2,50);
low = readOneByte(l_adr);	
	
retVal = high << 8;
retVal = retVal | low;

return retVal;


}

int16_t getRawAngle(void)
{
  return readtwoByte(_raw_ang_hi, _raw_ang_lo);
}