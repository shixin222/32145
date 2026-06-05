#ifndef __TEMP_H__
#define __TEMP_H__

#define FAN_ON_TEMP   31.0
#define FAN_OFF_TEMP  30.0

extern float current_temp;

void temp_init();
void temp_update();
void temp_reset();

#endif