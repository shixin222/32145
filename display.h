#ifndef __DISPLAY_H__
#define __DISPLAY_H__

#include <Arduino.h>

void display_init();
void display_update(bool key1_state, bool wifi_connected, String wifi_ip);

#endif
