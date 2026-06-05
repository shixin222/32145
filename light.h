#ifndef __LIGHT_H__
#define __LIGHT_H__

#include <Arduino.h>

#define ADC_PIN 1
#define LIGHT_ON_TH   1500
#define LIGHT_OFF_TH  1000

void light_init();

void light_update();

extern int light_adc;
extern float light_voltage;
#endif