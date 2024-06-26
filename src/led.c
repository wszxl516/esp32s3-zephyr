#include "led_colors.h"
#include "tasks_common.h"
#include <stdint.h>

LOG_MODULE_REGISTER(led_task);

#define LOG_LEVEL 7
#define STRIP_NODE DT_ALIAS(rgb_led)

#if DT_NODE_HAS_PROP(DT_ALIAS(rgb_led), chain_length)
#define STRIP_NUM_PIXELS DT_PROP(DT_ALIAS(rgb_led), chain_length)
#else
#error Unable to determine length of LED strip
#endif

#define DELAY_TIME K_MSEC(CONFIG_SAMPLE_LED_UPDATE_DELAY)


static const struct device *const strip = DEVICE_DT_GET(STRIP_NODE);

void led_task(void *args, void *arg1, void *arg2) {
  int rc;
  if (device_is_ready(strip)) {
    LOG_INF("Found LED strip device %s", strip->name);
  } else {
    LOG_ERR("LED strip device %s is not ready", strip->name);
    return;
  }
  const char *task_name = CURRENT_NAME();
  led_rgb pixel = {0};
  led_hsv hsv ={.hue = 0, .sat = 0xff, .val = 0xa};
  while (1) {
    for (hsv.hue = 0; hsv.hue < UINT8_MAX; hsv.hue++) {
      hsv2rgb(&hsv, &pixel);
      if ((rc = led_strip_update_rgb(strip, &pixel, 1)))
      {
        LOG_ERR("couldn't update strip: %d", rc);
      }
      k_msleep(10);
    }
    LOG_INF("task: %s is running! \n", task_name);
  }
  return;
}

void hsv2rgb(led_hsv *hsv, led_rgb *rgb) {
  uint16_t v = hsv->val;
  uint16_t s = hsv->sat;
  uint16_t f = (hsv->hue * 2 % 85) * 3;

  uint16_t p = v * (255 - s) / 255;
  uint16_t q = v * (255 - (s * f) / 255) / 255;
  uint16_t t = v * (255 - (s * (255 - f)) / 255) / 255;
  switch (hsv->hue) {
  case 0 ... 42:
    rgb->r = v;
    rgb->g = t;
    rgb->b = p;
    break;
  case 43 ... 84:
    rgb->r = q;
    rgb->g = v;
    rgb->b = p;
    break;
  case 85 ... 127:
    rgb->r = p;
    rgb->g = v;
    rgb->b = t;
    break;
  case 128 ... 169:
    rgb->r = p;
    rgb->g = q;
    rgb->b = v;
    break;
  case 170 ... 212:
    rgb->r = t;
    rgb->g = p;
    rgb->b = v;
    break;
  case 213 ... 254:
    rgb->r = v;
    rgb->g = p;
    rgb->b = q;
    break;
  case 255:
    rgb->r = v;
    rgb->g = t;
    rgb->b = p;
    break;
  }
}