#ifndef __LED_COLORS__
#define __LED_COLORS__
#include "zephyr/drivers/led_strip.h"
#include <stdint.h>

typedef struct {
   uint8_t hue;
    uint8_t sat;
   uint8_t val;
} led_hsv;

typedef struct led_rgb led_rgb;

void hsv2rgb(led_hsv *hsv, led_rgb *rgb);
#endif //__LED_COLORS__