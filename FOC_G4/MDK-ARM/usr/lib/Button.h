#ifndef __BUTTON_H__
#define __BUTTON_H__

#include "app.h"

#define ButtonPin1 GPIO_PIN_0
#define ButtonPin2 GPIO_PIN_14
#define ButtonPin3 GPIO_PIN_4
#define ButtonPin4 GPIO_PIN_15


extern  unsigned char button1count;
extern  unsigned char button2count;
extern  unsigned char button3count;
extern  unsigned char button4count;


void ButtonInit();

#endif