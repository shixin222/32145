#include "rgb_led.h"
#include "wifi_manager.h"
#include <Adafruit_NeoPixel.h>

#define WS2812_PIN 0
#define NUM_LEDS   1

static Adafruit_NeoPixel strip(NUM_LEDS, WS2812_PIN, NEO_GRB + NEO_KHZ800);

// 开机闪烁
static bool  booting     = true;
static int   boot_phase  = 0;   // 0=R, 1=G, 2=B
static int   boot_count  = 0;   // 已闪烁轮数
static unsigned long boot_t = 0;

// 状态 LED
static unsigned long led_t    = 0;
static bool          led_on   = false;

void rgb_led_init()
{
    strip.begin();
    strip.setBrightness(32);  // 不刺眼
    strip.clear();
    strip.show();

    booting    = true;
    boot_phase = 0;
    boot_count = 0;
    boot_t     = millis();
}

void rgb_led_set(uint8_t r, uint8_t g, uint8_t b)
{
    strip.setPixelColor(0, strip.Color(r, g, b));
    strip.show();
}

void rgb_led_update(int wifi_state, bool wifi_connected)
{
    // ===== 开机彩虹闪烁 =====
    if (booting) {
        if (millis() - boot_t > 200) {
            boot_t = millis();
            boot_phase++;
            if (boot_phase > 2) {
                boot_phase = 0;
                boot_count++;
            }

            // 2 轮后结束
            if (boot_count >= 2) {
                booting = false;
                strip.clear();
                strip.show();
                return;
            }

            switch (boot_phase) {
                case 0: rgb_led_set(255, 0, 0); break;   // 红
                case 1: rgb_led_set(0, 255, 0); break;   // 绿
                case 2: rgb_led_set(0, 0, 255); break;   // 蓝
            }
        }
        return;
    }

    // ===== WiFi 状态指示 =====
    if (wifi_connected) {
        // 已连接 → 绿色常亮
        rgb_led_set(0, 255, 0);
        return;
    }

    if (wifi_state == WF_CONNECTING) {
        // 连接中 → 蓝色 500ms 闪烁
        if (millis() - led_t > 500) {
            led_t  = millis();
            led_on = !led_on;
            if (led_on)
                rgb_led_set(0, 0, 255);
            else
                rgb_led_set(0, 0, 0);
        }
        return;
    }

    // 未联网 / 断网 / 失败 → 红色 1 秒慢闪
    if (millis() - led_t > 1000) {
        led_t  = millis();
        led_on = !led_on;
        if (led_on)
            rgb_led_set(255, 0, 0);
        else
            rgb_led_set(0, 0, 0);
    }
}
