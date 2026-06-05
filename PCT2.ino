#include "version.h"
#include "key.h"
#include "relay.h"
#include "exti.h"
#include "light.h"
#include "temp.h"
#include "display.h"
#include "wifi_manager.h"
#include "rgb_led.h"

void setup()
{
    Serial.begin(115200);
    Serial.println("系统启动");

    key_init();
    relay_init();
    exti_init();
    display_init();
    light_init();
    temp_init();
    rgb_led_init();
    wifi_init();

    rgb_led_update(wifi_state, wifi_connected);
}

void loop()
{
    wifi_loop();
    wifi_serial_input();

    static unsigned long led_timer = 0;
    if (millis() - led_timer > 200) {
        led_timer = millis();
        rgb_led_update(wifi_state, wifi_connected);
    }

    key2_scan();
    bool k1 = digitalRead(key1_pin);

    // 总闸
    if (!k1) {
        relay_mode = 0;
        auto_mode  = true;
        temp_reset();
        relay_all_off();
    }
    else {
        if (auto_mode) {
            light_update();
            temp_update();
        } else {
            relay_update(k1, relay_mode);
        }
    }

    static bool last_auto = auto_mode;
    if (last_auto != auto_mode) {
        last_auto = auto_mode;
        Serial.print("[模式] ");
        Serial.println(auto_mode ? "自动" : "手动");
    }

    // 每秒刷屏
    static unsigned long ts = 0;
    if (millis() - ts > 1000) {
        ts = millis();
        display_update(k1, wifi_connected, wifi_ip);
    }

    delay(10);
}
