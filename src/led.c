#include "tasks_common.h"
#include <string.h>

LOG_MODULE_REGISTER(led_task);

#define LOG_LEVEL 7
#define STRIP_NODE DT_ALIAS(led_strip)

#if DT_NODE_HAS_PROP(DT_ALIAS(led_strip), chain_length)
#define STRIP_NUM_PIXELS DT_PROP(DT_ALIAS(led_strip), chain_length)
#else
#error Unable to determine length of LED strip
#endif

#define DELAY_TIME K_MSEC(CONFIG_SAMPLE_LED_UPDATE_DELAY)

#define RGB(_r, _g, _b) {.r = (_r), .g = (_g), .b = (_b)}

static const struct led_rgb colors[] = {
    RGB(0x0f, 0x00, 0x00), /* red */
    RGB(0x00, 0x0f, 0x00), /* green */
    RGB(0x00, 0x00, 0x0f), /* blue */
};

static struct led_rgb pixels[STRIP_NUM_PIXELS];

static const struct device *const strip = DEVICE_DT_GET(STRIP_NODE);

void led_task(void *args, void *arg1, void *arg2) {
  size_t color = 0;
  int rc;

  if (device_is_ready(strip)) {
    LOG_INF("Found LED strip device %s", strip->name);
  } else {
    LOG_ERR("LED strip device %s is not ready", strip->name);
    return;
  }
  LOG_INF("Displaying pattern on strip");
  const char *task_name = CURRENT_NAME();
  while (1) {
    for (size_t cursor = 0; cursor < ARRAY_SIZE(pixels); cursor++) {
      memset(&pixels, 0x00, sizeof(pixels));
      memcpy(&pixels[cursor], &colors[color], sizeof(struct led_rgb));
      rc = led_strip_update_rgb(strip, pixels, STRIP_NUM_PIXELS);
      if (rc) {
        LOG_ERR("couldn't update strip: %d", rc);
      }
      k_msleep(1000);
    }
    LOG_INF("in task: %s\n", task_name);
    color = (color + 1) % ARRAY_SIZE(colors);
  }
  return;
}
