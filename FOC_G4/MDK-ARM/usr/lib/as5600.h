#ifndef _AS5600_H_
#define _AS5600_H_
#include "app.h"
#define AS5600_ADDRESS_MAG           0x36

typedef enum
{
/* set i2c address */ 
  _ams5600_Address = 0x36,
  _zmco = 0x00,
  _zpos_hi = 0x01,
  _zpos_lo = 0x02,
  _mpos_hi = 0x03,
  _mpos_lo = 0x04,
  _mang_hi = 0x05,
  _mang_lo = 0x06,
  _conf_hi = 0x07,    
  _conf_lo = 0x08,
  _raw_ang_hi = 0x0c,
  _raw_ang_lo = 0x0d,//这两个地址要注意
  _ang_hi = 0x0e,
  _ang_lo = 0x0f,
  _stat = 0x0b,
  _agc = 0x1a,
  _mag_hi = 0x1b,
  _mag_lo = 0x1c,
  _burn = 0xff
}AMS5600Registers_t;


void test_as5600();
uint8_t readOneByte(uint8_t in_adr);
uint16_t readtwoByte(uint8_t h_adr , uint8_t l_adr);
int16_t getRawAngle(void);

#endif