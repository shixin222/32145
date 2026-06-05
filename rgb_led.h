#ifndef __RGB_LED_H__
#define __RGB_LED_H__

#include <Arduino.h>

/**
 * 初始化 WS2812 LED (GPIO0, 1颗)
 */
void rgb_led_init();

/**
 * 主循环中调用，根据 WiFi 状态自动切换 LED 行为
 * @param wifi_state     WiFi 状态 (WF_CONNECTING / WF_CONNECTED / ...)
 * @param wifi_connected 是否已联网
 */
void rgb_led_update(int wifi_state, bool wifi_connected);

/**
 * 直接设置颜色 RGB(0-255)
 */
void rgb_led_set(uint8_t r, uint8_t g, uint8_t b);

#endif
