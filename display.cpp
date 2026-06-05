#include "display.h"
#include "exti.h"         // auto_mode, relay_mode
#include "temp.h"         // current_temp, FAN_ON_TEMP, FAN_OFF_TEMP
#include "light.h"        // light_adc, LIGHT_ON_TH, LIGHT_OFF_TH
#include "relay.h"        // relay1_pin, relay2_pin
#include <U8g2lib.h>

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE, 5, 4);

void display_init()
{
    u8g2.begin();
    u8g2.enableUTF8Print();
}

void display_update(bool key1_state, bool wifi_connected, String wifi_ip)
{
    bool light_on = digitalRead(relay1_pin);
    bool fan_on   = digitalRead(relay2_pin);

    int   light_th = 200;
    float temp_th  = fan_on ? FAN_ON_TEMP : FAN_OFF_TEMP;

    unsigned long inv_adc = 4095 - light_adc;
    unsigned long lux     = (inv_adc * inv_adc) / 30000;

    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_wqy12_t_gb2312);

    // 第1行: 模式 + 总闸
    u8g2.setCursor(2, 10);
    u8g2.print("模式：");
    u8g2.print(auto_mode ? "自动" : "手动");
    u8g2.setCursor(72, 10);
    u8g2.print("总闸：");
    u8g2.print(key1_state ? "ON" : "OFF");

    // 第2行: 光照
    u8g2.setCursor(2, 22);
    u8g2.print("光照：");
    u8g2.print((unsigned int)lux);
    u8g2.print(" / ");
    u8g2.print(light_th);

    // 第3行: 温度
    u8g2.setCursor(2, 34);
    u8g2.print("温度：");
    u8g2.print(current_temp, 1);
    u8g2.print(" / ");
    u8g2.print(temp_th, 1);

    // 第4行: 灯光 + 风扇
    u8g2.setCursor(2, 46);
    u8g2.print("灯光：");
    u8g2.print(light_on ? "ON" : "OFF");
    u8g2.setCursor(72, 46);
    u8g2.print("风扇：");
    u8g2.print(fan_on ? "ON" : "OFF");

    // 第5行: WiFi
    u8g2.setCursor(2, 58);
    u8g2.print("WiFi：");
    if (wifi_connected) {
        // IP 太长，截断显示
        if (wifi_ip.length() > 13)
            wifi_ip = wifi_ip.substring(0, 13);
        u8g2.print(wifi_ip.c_str());
    } else {
        u8g2.print("未连接");
    }

    u8g2.sendBuffer();
}
